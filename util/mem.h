#ifndef _MEM_H_
#define _MEM_H_

#ifdef __TP3_MEM_DEBUG__

#define MALLOC(x) mem_malloc(x,__FILE__,__LINE__)
#define REALLOC(x,y) mem_realloc(x,y,__FILE__,__LINE__)
#define FREE(x) mem_free(x,__FILE__,__LINE__)

void *
mem_malloc (size_t size, char *file, int line); 
void *
mem_realloc (void *ptr, size_t size, char *file, int line); 

void
mem_free (void *ptr, char *file, int line);

#else

#define MALLOC(x) malloc(x)
#define REALLOC(x,y) realloc(x,y)
#define FREE(x) free(x)

#endif

#endif
