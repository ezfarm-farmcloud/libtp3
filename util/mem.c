#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

#ifdef __TP3_MEM_DEBUG__

void *
mem_malloc (size_t size, char *file, int line) {
	void *ptr;

	fprintf (stderr, "%s:%d alloc ", file, line);
	fflush (stderr);
	ptr = malloc (size);
	fprintf (stderr, "(%p) size (%d)\n", ptr, (int)size);
	return ptr;
}

void *
mem_realloc (void *ptr, size_t size, char *file, int line) {
	void *nptr;

	fprintf (stderr, "%s:%d alloc ", file, line);
	fflush (stderr);
	nptr = realloc (ptr, size);
	fprintf (stderr, "(%p) size (%d)\n", nptr, (int)size);
	return nptr;
}

void
mem_free (void *ptr, char *file, int line) {
	fprintf (stderr, "%s:%d free (%p)\n", file, line, ptr);
	free (ptr);
}

#endif
