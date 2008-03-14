/*
 * shared.h:  Shared memory handle, and daemon launching
 *
 * Author:
 *	Dick Porter (dick@ximian.com)
 *
 * (C) 2002-2006 Novell, Inc.
 */

#ifndef _WAPI_SHARED_H_
#define _WAPI_SHARED_H_

#include <mono/io-layer/wapi-private.h>

extern gboolean _wapi_shm_disabled;

extern void _wapi_shared_init (void);
extern void _wapi_shared_cleanup (void);

extern int _wapi_shm_sem_lock (int sem);
extern int _wapi_shm_sem_trylock (int sem);
extern int _wapi_shm_sem_unlock (int sem);

extern void _wapi_shared_lock_file (int fd);
extern void _wapi_shared_unlock_file (int fd);

extern void _wapi_shared_read_handle (int fd, gpointer data, guint32 datalen);
extern void _wapi_shared_write_handle (int fd, gpointer data, guint32 datalen);

extern int _wapi_shared_open_process (pid_t pid, gboolean locked,
				      gboolean create);
extern int _wapi_shared_open_thread (pid_t pid, pthread_t tid,
				     gboolean locked, gboolean create);
extern int _wapi_shared_open_named (gchar *name, gboolean locked,
				    gboolean create);

#endif /* _WAPI_SHARED_H_ */
