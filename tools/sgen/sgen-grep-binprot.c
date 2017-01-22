#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glib.h>

#define SGEN_BINARY_PROTOCOL
#define MONO_INTERNAL

#include <mono/metadata/sgen-protocol.h>

#define SGEN_PROTOCOL_EOF	255

static int
read_entry (FILE *in, void **data)
{
	unsigned char type;
	int size;

	if (fread (&type, 1, 1, in) != 1)
		return SGEN_PROTOCOL_EOF;
	switch (type) {
	case SGEN_PROTOCOL_COLLECTION: size = sizeof (SGenProtocolCollection); break;
	case SGEN_PROTOCOL_ALLOC: size = sizeof (SGenProtocolAlloc); break;
	case SGEN_PROTOCOL_ALLOC_PINNED: size = sizeof (SGenProtocolAlloc); break;
	case SGEN_PROTOCOL_ALLOC_DEGRADED: size = sizeof (SGenProtocolAlloc); break;
	case SGEN_PROTOCOL_COPY: size = sizeof (SGenProtocolCopy); break;
	case SGEN_PROTOCOL_PIN: size = sizeof (SGenProtocolPin); break;
	case SGEN_PROTOCOL_MARK: size = sizeof (SGenProtocolMark); break;
	case SGEN_PROTOCOL_WBARRIER: size = sizeof (SGenProtocolWBarrier); break;
	case SGEN_PROTOCOL_GLOBAL_REMSET: size = sizeof (SGenProtocolGlobalRemset); break;
	case SGEN_PROTOCOL_PTR_UPDATE: size = sizeof (SGenProtocolPtrUpdate); break;
	case SGEN_PROTOCOL_CLEANUP: size = sizeof (SGenProtocolCleanup); break;
	case SGEN_PROTOCOL_EMPTY: size = sizeof (SGenProtocolEmpty); break;
	case SGEN_PROTOCOL_THREAD_RESTART: size = sizeof (SGenProtocolThreadRestart); break;
	case SGEN_PROTOCOL_THREAD_REGISTER: size = sizeof (SGenProtocolThreadRegister); break;
	case SGEN_PROTOCOL_THREAD_UNREGISTER: size = sizeof (SGenProtocolThreadUnregister); break;
	case SGEN_PROTOCOL_MISSING_REMSET: size = sizeof (SGenProtocolMissingRemset); break;
	default: assert (0);
	}

	*data = malloc (size);
	if (fread (*data, size, 1, in) != 1)
		assert (0);

	return (int)type;
}

static void
print_entry (int type, void *data)
{
	switch (type) {
	case SGEN_PROTOCOL_COLLECTION: {
		SGenProtocolCollection *entry = data;
		printf ("collection generation %d\n", entry->generation);
		break;
	}
	case SGEN_PROTOCOL_ALLOC: {
		SGenProtocolAlloc *entry = data;
		printf ("alloc obj %p vtable %p size %d\n", entry->obj, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_ALLOC_PINNED: {
		SGenProtocolAlloc *entry = data;
		printf ("alloc pinned obj %p vtable %p size %d\n", entry->obj, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_ALLOC_DEGRADED: {
		SGenProtocolAlloc *entry = data;
		printf ("alloc degraded obj %p vtable %p size %d\n", entry->obj, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_COPY: {
		SGenProtocolCopy *entry = data;
		printf ("copy from %p to %p vtable %p size %d\n", entry->from, entry->to, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_PIN: {
		SGenProtocolPin *entry = data;
		printf ("pin obj %p vtable %p size %d\n", entry->obj, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_MARK: {
		SGenProtocolMark *entry = data;
		printf ("mark obj %p vtable %p size %d\n", entry->obj, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_WBARRIER: {
		SGenProtocolWBarrier *entry = data;
		printf ("wbarrier ptr %p value %p value_vtable %p\n", entry->ptr, entry->value, entry->value_vtable);
		break;
	}
	case SGEN_PROTOCOL_GLOBAL_REMSET: {
		SGenProtocolGlobalRemset *entry = data;
		printf ("global_remset ptr %p value %p value_vtable %p\n", entry->ptr, entry->value, entry->value_vtable);
		break;
	}
	case SGEN_PROTOCOL_PTR_UPDATE: {
		SGenProtocolPtrUpdate *entry = data;
		printf ("ptr_update ptr %p old_value %p new_value %p vtable %p size %d\n",
				entry->ptr, entry->old_value, entry->new_value, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_CLEANUP: {
		SGenProtocolCleanup *entry = data;
		printf ("cleanup ptr %p vtable %p size %d\n", entry->ptr, entry->vtable, entry->size);
		break;
	}
	case SGEN_PROTOCOL_EMPTY: {
		SGenProtocolEmpty *entry = data;
		printf ("empty start %p size %d\n", entry->start, entry->size);
		break;
	}
	case SGEN_PROTOCOL_THREAD_RESTART: {
		SGenProtocolThreadRestart *entry = data;
		printf ("thread_restart thread %p\n", entry->thread);
		break;
	}
	case SGEN_PROTOCOL_THREAD_REGISTER: {
		SGenProtocolThreadRegister *entry = data;
		printf ("thread_register thread %p\n", entry->thread);
		break;
	}
	case SGEN_PROTOCOL_THREAD_UNREGISTER: {
		SGenProtocolThreadUnregister *entry = data;
		printf ("thread_unregister thread %p\n", entry->thread);
		break;
	}
	case SGEN_PROTOCOL_MISSING_REMSET: {
		SGenProtocolMissingRemset *entry = data;
		printf ("missing_remset obj %p obj_vtable %p offset %d value %p value_vtable %p value_pinned %d\n",
				entry->obj, entry->obj_vtable, entry->offset, entry->value, entry->value_vtable, entry->value_pinned);
		break;
	}
	default:
		assert (0);
	}
}

static gboolean
matches_interval (gpointer ptr, gpointer start, int size)
{
	return ptr >= start && (char*)ptr < (char*)start + size;
}

static gboolean
is_match (gpointer ptr, int type, void *data)
{
	switch (type) {
	case SGEN_PROTOCOL_COLLECTION:
	case SGEN_PROTOCOL_THREAD_RESTART:
	case SGEN_PROTOCOL_THREAD_REGISTER:
	case SGEN_PROTOCOL_THREAD_UNREGISTER:
		return TRUE;
	case SGEN_PROTOCOL_ALLOC:
	case SGEN_PROTOCOL_ALLOC_PINNED:
	case SGEN_PROTOCOL_ALLOC_DEGRADED: {
		SGenProtocolAlloc *entry = data;
		return matches_interval (ptr, entry->obj, entry->size);
	}
	case SGEN_PROTOCOL_COPY: {
		SGenProtocolCopy *entry = data;
		return matches_interval (ptr, entry->from, entry->size) || matches_interval (ptr, entry->to, entry->size);
	}
	case SGEN_PROTOCOL_PIN: {
		SGenProtocolPin *entry = data;
		return matches_interval (ptr, entry->obj, entry->size);
	}
	case SGEN_PROTOCOL_MARK: {
		SGenProtocolMark *entry = data;
		return matches_interval (ptr, entry->obj, entry->size);
	}
	case SGEN_PROTOCOL_WBARRIER: {
		SGenProtocolWBarrier *entry = data;
		return ptr == entry->ptr || ptr == entry->value;
	}
	case SGEN_PROTOCOL_GLOBAL_REMSET: {
		SGenProtocolGlobalRemset *entry = data;
		return ptr == entry->ptr || ptr == entry->value;
	}
	case SGEN_PROTOCOL_PTR_UPDATE: {
		SGenProtocolPtrUpdate *entry = data;
		return ptr == entry->ptr ||
			matches_interval (ptr, entry->old_value, entry->size) ||
			matches_interval (ptr, entry->new_value, entry->size);
	}
	case SGEN_PROTOCOL_CLEANUP: {
		SGenProtocolCleanup *entry = data;
		return matches_interval (ptr, entry->ptr, entry->size);
	}
	case SGEN_PROTOCOL_EMPTY: {
		SGenProtocolEmpty *entry = data;
		return matches_interval (ptr, entry->start, entry->size);
	}
	case SGEN_PROTOCOL_MISSING_REMSET: {
		SGenProtocolMissingRemset *entry = data;
		return ptr == entry->obj || ptr == entry->value || ptr == (char*)entry->obj + entry->offset;
	}
	default:
		assert (0);
	}
}

int
main (int argc, char *argv[])
{
	int type;
	void *data;
	int num_nums = argc - 1;
	int i;
	long nums [num_nums];

	for (i = 0; i < num_nums; ++i)
		nums [i] = strtoul (argv [i + 1], NULL, 16);

	while ((type = read_entry (stdin, &data)) != SGEN_PROTOCOL_EOF) {
		gboolean match = FALSE;
		for (i = 0; i < num_nums; ++i) {
			if (is_match ((gpointer) nums [i], type, data)) {
				match = TRUE;
				break;
			}
		}
		if (match)
			print_entry (type, data);
		free (data);
	}

	return 0;
}
