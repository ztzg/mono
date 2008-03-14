/*
 * shared.c:  Shared memory handling, and daemon launching
 *
 * Author:
 *	Dick Porter (dick@ximian.com)
 *
 * (C) 2002-2006 Novell, Inc.
 */


#include <config.h>
#include <glib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/utsname.h>

#include <mono/io-layer/wapi.h>
#include <mono/io-layer/wapi-private.h>
#include <mono/io-layer/shared.h>
#include <mono/io-layer/handles-private.h>

#define DEBUG

#ifdef DISABLE_SHARED_HANDLES
gboolean _wapi_shm_disabled = TRUE;
#else
gboolean _wapi_shm_disabled = FALSE;
#endif

#define _WAPI_HANDLE_FILE_VERSION 0

static gchar processes_dir[_POSIX_PATH_MAX];
static gchar threads_dir[_POSIX_PATH_MAX];
static gchar named_dir[_POSIX_PATH_MAX];
static gchar seminfo_file[_POSIX_PATH_MAX];
static int seminfo_fd;

static void check_disabled (void)
{
	if (_wapi_shm_disabled || g_getenv ("MONO_DISABLE_SHM")) {
		const char* val = g_getenv ("MONO_DISABLE_SHM");
		if (val == NULL || *val == '1' || *val == 'y' || *val == 'Y') {
			_wapi_shm_disabled = TRUE;
		}
	}
}

static gboolean _wapi_mkdirhier (const gchar *dir, mode_t mode)
{
	gchar *working = g_strdup (dir), *p = working;
	struct stat statbuf;

	if (*p == '/') {
		p++;
	}
	
	do {
		/* Find next separator */
		while(*p && *p != '/') {
			p++;
		}
		
		if (!*p) {
			p = NULL;
		} else {
			*p = '\0';
		}

		/* check if 'working' (as far as 'p') refers to an
		 * non-existant file, or existing directory
		 */
		if (access (working, F_OK) != 0) {
			if (mkdir (working, mode) == -1) {
				int errno_save = errno;
				g_free (working);
				errno = errno_save;
				return(-1);
			}
		} else if ((stat (working, &statbuf) == 0) &&
			   (!S_ISDIR (statbuf.st_mode))) {
			g_free (working);
			errno = ENOTDIR;
			return(-1);
		}

		/* Put the separator back we previously turned into a
		 * '\0', and skip over contiguous separators
		 */
		if (p) {
			*p++ = '/';
			while (*p == '/') {
				p++;
			}
		}
	} while(p);

	g_free (working);
	
	return(0);
}

static void locate_shared_files (void)
{
	gchar machine_name[256], *name, *base_dir, *file;
	const gchar *fake_name, *wapi_dir;
	struct utsname ubuf;
	int ret, len;

	ret = uname (&ubuf);
	if (ret == -1) {
		ubuf.machine[0] = '\0';
		ubuf.sysname[0] = '\0';
	} else {
		g_strdelimit (ubuf.sysname, "/", '_');
		g_strdelimit (ubuf.machine, "/", '_');
	}

	fake_name = g_getenv ("MONO_SHARED_HOSTNAME");
	if (fake_name == NULL) {
		if (gethostname (machine_name, sizeof(machine_name)) != 0) {
			machine_name[0] = '\0';
		}
	} else {
		len = MIN (strlen (fake_name), sizeof(machine_name) - 1);
		strncpy (machine_name, fake_name, len);
		machine_name[len] = '\0';
	}
	
	name = g_strdup_printf ("%s-%s-%s-%d", machine_name, ubuf.sysname,
				ubuf.machine, _WAPI_HANDLE_FILE_VERSION);
	
	wapi_dir = g_getenv ("MONO_SHARED_DIR");
	if (wapi_dir == NULL) {
		base_dir = g_build_filename (g_get_home_dir (), ".wapi",
					     "files", name, NULL);
	} else {
		base_dir = g_build_filename (wapi_dir, ".wapi", "files",
					     name, NULL);
	}
	g_free (name);
	
	file = g_build_filename (base_dir, "processes", NULL);
	g_snprintf (processes_dir, _POSIX_PATH_MAX, "%s", file);
	g_free (file);
	if (_wapi_mkdirhier (processes_dir, 0700) == -1) {
		g_warning ("%s: Can't create processes data dir %s: %s.  Reverting to unshared mode.", __func__, processes_dir, g_strerror (errno));
		_wapi_shm_disabled = TRUE;
	}
	
	file = g_build_filename (base_dir, "threads", NULL);
	g_snprintf (threads_dir, _POSIX_PATH_MAX, "%s", file);
	g_free (file);
	if (_wapi_mkdirhier (threads_dir, 0700) == -1) {
		g_warning ("%s: Can't create threads data dir %s: %s.  Reverting to unshared mode.", __func__, threads_dir, g_strerror (errno));
		_wapi_shm_disabled = TRUE;
	}
	
	file = g_build_filename (base_dir, "named_items", NULL);
	g_snprintf (named_dir, _POSIX_PATH_MAX, "%s", file);
	g_free (file);
	if (_wapi_mkdirhier (named_dir, 0700) == -1) {
		g_warning ("%s: Can't create named objects data dir %s: %s.  Reverting to unshared mode.", __func__, named_dir, g_strerror (errno));
		_wapi_shm_disabled = TRUE;
	}
	
	file = g_build_filename (base_dir, "semaphores", NULL);
	g_snprintf (seminfo_file, _POSIX_PATH_MAX, "%s", file);
	g_free (file);
	seminfo_fd = open (seminfo_file, O_CREAT|O_RDWR, 0600);
	if(seminfo_fd == -1) {
		g_warning ("%s: Can't open semaphore data file %s: %s.  Reverting to unshared mode.", __func__, seminfo_file, g_strerror (errno));
		_wapi_shm_disabled = TRUE;
	}

	/* Locks aren't released on exec, so make sure the file
	 * descriptor will close (which does release the lock.)
	 */
	fcntl (seminfo_fd, F_SETFD, FD_CLOEXEC);
	/* ignore fcntl() errors for now */
}

static void delete_directory_contents (gchar *dirname)
{
	DIR *dir = NULL;
	struct dirent *entry;
	
	dir = opendir (dirname);
	if (dir == NULL) {
		return;
	}
	
	while((entry = readdir (dir)) != NULL) {
		if (strcmp (entry->d_name, ".") &&
		    strcmp (entry->d_name, "..")) {
			gchar *file = g_build_filename (dirname, entry->d_name,
							NULL);
		
#ifdef DEBUG
			g_message ("%s: deleting %s", __func__, file);
#endif
			unlink (file);
			g_free (file);
		}
	}
	closedir (dir);
}

static void delete_shared_files (void)
{
	delete_directory_contents (processes_dir);
	delete_directory_contents (threads_dir);
	delete_directory_contents (named_dir);
}

void _wapi_shared_lock_file (int fd)
{
	int ret;
	struct flock lockbuf;

	if (_wapi_shm_disabled) {
		return;
	}

	lockbuf.l_type = F_WRLCK;
	lockbuf.l_whence = SEEK_SET;
	lockbuf.l_start = 0;
	lockbuf.l_len = 0;
	
	do {
		ret = fcntl (fd, F_SETLKW, &lockbuf);
	} while(ret == -1 && errno == EINTR);
}

void _wapi_shared_unlock_file (int fd)
{
	int ret;
	struct flock lockbuf;
	
	if (_wapi_shm_disabled) {
		return;
	}

	lockbuf.l_type = F_UNLCK;
	lockbuf.l_whence = SEEK_SET;
	lockbuf.l_start = 0;
	lockbuf.l_len = 0;
	
	do {
		ret = fcntl (fd, F_SETLK, &lockbuf);
	} while(ret == -1 && errno == EINTR);
}

void _wapi_shared_read_handle (int fd, gpointer data, guint32 datalen)
{
	int ret;
	
	lseek (fd, 0, SEEK_SET);
	
	ret = read (fd, data, datalen);
	if (ret == -1) {
		g_warning ("%s: Read error on %d: %s", __func__, fd,
			   g_strerror (errno));
	}
}

void _wapi_shared_write_handle (int fd, gpointer data, guint32 datalen)
{
	int ret;
	
	lseek (fd, 0, SEEK_SET);
	
	ret = write (fd, data, datalen);
	if (ret == -1) {
		g_warning ("%s: Write error on %d: %s", __func__, fd,
			   g_strerror (errno));
	}
}

static key_t read_key (void)
{
	int ret;
	key_t key;
	char *keybuf = (char *)&key;
	
	lseek (seminfo_fd, 0, SEEK_SET);
	
	ret = read (seminfo_fd, keybuf, sizeof(key_t));
	if (ret == -1) {
		g_warning ("%s: Read error on %s: %s", __func__, seminfo_file,
			   g_strerror (errno));
	}
	
	return(key);
}

static void write_key (key_t key)
{
	int ret;
	
	lseek (seminfo_fd, 0, SEEK_SET);
	
	ret = write (seminfo_fd, (void *)&key, sizeof(key_t));
	if (ret == -1) {
		g_warning ("%s: Write error on %s: %s", __func__, seminfo_file,
			   g_strerror (errno));
	}
}

static void shm_semaphores_init (void)
{
	key_t key;
	key_t oldkey;
	
	/* Yet more barmy API - this union is a well-defined parameter
	 * in a syscall, yet I still have to define it here as it
	 * doesn't appear in a header
	 */
	union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	} defs;
	ushort def_vals[_WAPI_SHARED_SEM_COUNT];
	int i;
	int retries = 0;
	
	for (i = 0; i < _WAPI_SHARED_SEM_COUNT; i++) {
		def_vals[i] = 1;
	}

	/* Process count must start at '0' - the 1 for all the others
	 * sets the semaphore to "unlocked"
	 */
	def_vals[_WAPI_SHARED_SEM_PROCESS_COUNT] = 0;
	
	defs.array = def_vals;
	
	key = ftok (seminfo_file, 'M');

	_wapi_shared_lock_file (seminfo_fd);
	
again:
	retries++;
	oldkey = read_key ();

	if (oldkey == 0) {
#ifdef DEBUG
		g_message ("%s: Creating with new key (0x%x)", __func__, key);
#endif

		/* The while loop attempts to make some sense of the
		 * bonkers 'think of a random number' method of
		 * picking a key without collision with other
		 * applications
		 */
		while ((_wapi_sem_id = semget (key, _WAPI_SHARED_SEM_COUNT,
					       IPC_CREAT | IPC_EXCL | 0600)) == -1) {
			if (errno == ENOMEM) {
				g_error ("%s: semget error: %s", __func__,
					    g_strerror (errno));
			} else if (errno == ENOSPC) {
				g_error ("%s: semget error: %s.  Try deleting some semaphores with ipcs and ipcrm\nor increase the maximum number of semaphore in the system.", __func__, g_strerror (errno));
			} else if (errno != EEXIST) {
				if (retries > 3)
					g_warning ("%s: semget error: %s key 0x%x - trying again", __func__,
							g_strerror (errno), key);
			}
			
			key++;
#ifdef DEBUG
			g_message ("%s: Got (%s), trying with new key (0x%x)",
				   __func__, g_strerror (errno), key);
#endif
		}
		/* Got a semaphore array, so initialise it and install
		 * the key into the shared file
		 */
		
		if (semctl (_wapi_sem_id, 0, SETALL, defs) == -1) {
			if (retries > 3)
				g_warning ("%s: semctl init error: %s - trying again", __func__, g_strerror (errno));

			/* Something went horribly wrong, so try
			 * getting a new set from scratch
			 */
			semctl (_wapi_sem_id, 0, IPC_RMID);
			goto again;
		}

		write_key (key);
		goto done;
	}
	
#ifdef DEBUG
	g_message ("%s: Trying with old key 0x%x", __func__, oldkey);
#endif

	_wapi_sem_id = semget (oldkey, _WAPI_SHARED_SEM_COUNT, 0600);
	if (_wapi_sem_id == -1) {
		if (retries > 3)
			g_warning ("%s: semget error opening old key 0x%x (%s) - trying again",
					__func__, oldkey,g_strerror (errno));

		/* Someone must have deleted the semaphore set, so
		 * blow away the bad key and try again
		 */
		write_key (0);
		
		goto again;
	}

  done:
#ifdef DEBUG
	g_message ("%s: Incrementing the process count (%d)", __func__, _wapi_getpid ());
#endif

	/* We only ever _unlock_ this semaphore, letting the kernel
	 * restore (ie decrement) this unlock when this process exits.
	 */
	_wapi_shm_sem_unlock (_WAPI_SHARED_SEM_PROCESS_COUNT);

#ifdef DEBUG
	g_message ("%s: Process count is now %d (%d)", __func__, semctl (_wapi_sem_id, _WAPI_SHARED_SEM_PROCESS_COUNT, GETVAL), _wapi_getpid ());
#endif
	
	_wapi_shared_unlock_file (seminfo_fd);
}

static mono_mutex_t noshm_sems[_WAPI_SHARED_SEM_COUNT];

static void noshm_semaphores_init (void)
{
	int i;

	for (i = 0; i < _WAPI_SHARED_SEM_COUNT; i++) {
		mono_mutex_init (&noshm_sems[i], NULL);
	}
}

static void shm_semaphores_remove (void)
{
	int proc_count;
	
#ifdef DEBUG
	g_message ("%s: Checking process count (%d)", __func__,
		   _wapi_getpid ());
#endif
	
	_wapi_shared_lock_file (seminfo_fd);
	
	proc_count = semctl (_wapi_sem_id, _WAPI_SHARED_SEM_PROCESS_COUNT,
			     GETVAL);

	g_assert (proc_count > 0);
	if (proc_count == 1) {
		/* Just us, so blow away the semaphores and the shared
		 * files
		 */
#ifdef DEBUG
		g_message ("%s: Removing semaphores! (%d)", __func__,
			   _wapi_getpid ());
#endif

		semctl (_wapi_sem_id, 0, IPC_RMID);
		write_key (0);

		/* Delete all shared data files, as there are no more
		 * processes that can be using them.  (This can't
		 * race, as we're holding the file lock on the
		 * semaphore file.)
		 */
		delete_shared_files ();
	}

	_wapi_shared_unlock_file (seminfo_fd);
}

static void noshm_semaphores_remove (void)
{
	/* No need to do anything */
}

static void _wapi_shm_semaphores_init (void)
{
	if (_wapi_shm_disabled) {
		noshm_semaphores_init ();
	} else {
		shm_semaphores_init ();
	}
}

static void _wapi_shm_semaphores_remove (void)
{
	if (_wapi_shm_disabled) {
		noshm_semaphores_remove ();
	} else {
		shm_semaphores_remove ();
	}
}

static int shm_sem_lock (int sem)
{
	struct sembuf ops;
	int ret;
	
#ifdef DEBUG
	g_message ("%s: locking sem %d", __func__, sem);
#endif

	ops.sem_num = sem;
	ops.sem_op = -1;
	ops.sem_flg = SEM_UNDO;
	
  retry:
	do {
		ret = semop (_wapi_sem_id, &ops, 1);
	} while (ret == -1 && errno == EINTR);

	if (ret == -1) {
		/* EINVAL covers the case when the semaphore was
		 * deleted before we started the semop
		 */
		if (errno == EIDRM || errno == EINVAL) {
			/* Someone blew away this semaphore set, so
			 * get a new one and try again
			 */
#ifdef DEBUG
			g_message ("%s: Reinitialising the semaphores!",
				   __func__);
#endif

			_wapi_shm_semaphores_init ();
			goto retry;
		}
		
		/* Turn this into a pthreads-style return value */
		ret = errno;
	}
	
#ifdef DEBUG
	g_message ("%s: returning %d (%s)", __func__, ret, g_strerror (ret));
#endif
	
	return(ret);
}

static int noshm_sem_lock (int sem)
{
	int ret;
	
#ifdef DEBUG
	g_message ("%s: locking nosem %d", __func__, sem);
#endif
	
	ret = mono_mutex_lock (&noshm_sems[sem]);
	
	return(ret);
}

static int shm_sem_trylock (int sem)
{
	struct sembuf ops;
	int ret;
	
#ifdef DEBUG
	g_message ("%s: trying to lock sem %d", __func__, sem);
#endif
	
	ops.sem_num = sem;
	ops.sem_op = -1;
	ops.sem_flg = IPC_NOWAIT | SEM_UNDO;
	
  retry:
	do {
		ret = semop (_wapi_sem_id, &ops, 1);
	} while (ret == -1 && errno == EINTR);

	if (ret == -1) {
		/* EINVAL covers the case when the semaphore was
		 * deleted before we started the semop
		 */
		if (errno == EIDRM || errno == EINVAL) {
			/* Someone blew away this semaphore set, so
			 * get a new one and try again
			 */
#ifdef DEBUG
			g_message ("%s: Reinitialising the semaphores!",
				   __func__);
#endif

			_wapi_shm_semaphores_init ();
			goto retry;
		}
		
		/* Turn this into a pthreads-style return value */
		ret = errno;
	}
	
	if (ret == EAGAIN) {
		/* But pthreads uses this code instead */
		ret = EBUSY;
	}
	
#ifdef DEBUG
	g_message ("%s: returning %d (%s)", __func__, ret, g_strerror (ret));
#endif
	
	return(ret);
}

static int noshm_sem_trylock (int sem)
{
	int ret;
	
#ifdef DEBUG
	g_message ("%s: trying to lock nosem %d", __func__, sem);
#endif
	
	ret = mono_mutex_trylock (&noshm_sems[sem]);
	
	return(ret);
}

static int shm_sem_unlock (int sem)
{
	struct sembuf ops;
	int ret;
	
#ifdef DEBUG
	g_message ("%s: unlocking sem %d", __func__, sem);
#endif
	
	ops.sem_num = sem;
	ops.sem_op = 1;
	ops.sem_flg = SEM_UNDO;
	
  retry:
	do {
		ret = semop (_wapi_sem_id, &ops, 1);
	} while (ret == -1 && errno == EINTR);

	if (ret == -1) {
		/* EINVAL covers the case when the semaphore was
		 * deleted before we started the semop
		 */
		if (errno == EIDRM || errno == EINVAL) {
			/* Someone blew away this semaphore set, so
			 * get a new one and try again (we can't just
			 * assume that the semaphore is now unlocked)
			 */
#ifdef DEBUG
			g_message ("%s: Reinitialising the semaphores!",
				   __func__);
#endif

			_wapi_shm_semaphores_init ();
			goto retry;
		}
		
		/* Turn this into a pthreads-style return value */
		ret = errno;
	}
	
#ifdef DEBUG
	g_message ("%s: returning %d (%s)", __func__, ret, g_strerror (ret));
#endif

	return(ret);
}

static int noshm_sem_unlock (int sem)
{
	int ret;
	
#ifdef DEBUG
	g_message ("%s: unlocking nosem %d", __func__, sem);
#endif
	
	ret = mono_mutex_unlock (&noshm_sems[sem]);
	
	return(ret);
}

int _wapi_shm_sem_lock (int sem)
{
	if (_wapi_shm_disabled) {
		return(noshm_sem_lock (sem));
	} else {
		return(shm_sem_lock (sem));
	}
}

int _wapi_shm_sem_trylock (int sem)
{
	if (_wapi_shm_disabled) {
		return(noshm_sem_trylock (sem));
	} else {
		return(shm_sem_trylock (sem));
	}
}

int _wapi_shm_sem_unlock (int sem)
{
	if (_wapi_shm_disabled) {
		return(noshm_sem_unlock (sem));
	} else {
		return(shm_sem_unlock (sem));
	}
}

void _wapi_shared_init (void)
{
	check_disabled ();
	
	locate_shared_files ();
	_wapi_shm_semaphores_init ();
}

void _wapi_shared_cleanup (void)
{
	_wapi_shm_semaphores_remove ();
}

static int open_private (gchar *shared_filename, gchar *private_filename,
			 gboolean locked, gboolean create)
{
	int ret, fd, oflags;
	
	if (create) {
		oflags = O_CREAT|O_RDWR;
	} else {
		oflags = O_RDWR;
	}
	
	fd = open (shared_filename, oflags, 0600);
	if (fd == -1) {
		return(-1);
	}

	ret = link (shared_filename, private_filename);
	if (ret == -1) {
		close (fd);
		return(-1);
	}

	close (fd);
	
	fd = open (private_filename, O_RDWR, 0600);
	if (fd == -1) {
		return(-1);
	}

	/* Locks aren't released on exec, so make sure the file
	 * descriptor will close (which does release the lock.)
	 */
	ret = fcntl (fd, F_SETFD, FD_CLOEXEC);
	if (ret == -1) {
		close (fd);
		return(-1);
	}

	/* Now we've opened the process-specific link to the file we
	 * want, unlink it so it gets automatically cleaned up when
	 * we've finished with it
	 */
	unlink (private_filename);
	
	if (locked) {
		_wapi_shared_lock_file (fd);
	}

	return(fd);
}


int _wapi_shared_open_process (pid_t pid, gboolean locked, gboolean create)
{
	gchar filepart[_POSIX_PATH_MAX], *file;
	gchar private_filepart[_POSIX_PATH_MAX], *private_file;
	int fd;
	
	g_snprintf (filepart, _POSIX_PATH_MAX, "%d", pid);
	file = g_build_filename (processes_dir, filepart, NULL);

	g_snprintf (private_filepart, _POSIX_PATH_MAX, ".%d#%d", pid,
		    _wapi_getpid ());
	private_file = g_build_filename (processes_dir, private_filepart,
					 NULL);
	
	fd = open_private (file, private_file, locked, create);
	
	g_free (file);
	g_free (private_file);
	
	return(fd);
}

int _wapi_shared_open_thread (pid_t pid, pthread_t tid, gboolean locked,
			      gboolean create)
{
	gchar filepart[_POSIX_PATH_MAX], *file;
	gchar private_filepart[_POSIX_PATH_MAX], *private_file;
	int fd;
	
	g_snprintf (filepart, _POSIX_PATH_MAX, "%d-%ld", pid, tid);
	file = g_build_filename (threads_dir, filepart, NULL);
	
	g_snprintf (private_filepart, _POSIX_PATH_MAX, ".%d-%ld#%d", pid, tid,
		    _wapi_getpid ());
	private_file = g_build_filename (threads_dir, private_filepart, NULL);
	
	fd = open_private (file, private_file, locked, create);
	
	g_free (file);
	g_free (private_file);
	
	return(fd);
}

int _wapi_shared_open_named (gchar *name, gboolean locked, gboolean create)
{
	gchar *file;
	gchar private_filepart[_POSIX_PATH_MAX], *private_file;
	int fd;
	
	file = g_build_filename (named_dir, name, NULL);
	
	g_snprintf (private_filepart, _POSIX_PATH_MAX, ".%s#%d", name, _wapi_getpid ());
	private_file = g_build_filename (named_dir, private_filepart, NULL);
	
	fd = open_private (file, private_file, locked, create);

	g_free (file);
	g_free (private_file);
	
	return(fd);
}
