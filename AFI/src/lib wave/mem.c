/* mem.c  -  memory management functions for wf library */

#include <malloc.h>
#include "wave.h"

static void * (*wfAllocFunc)(size_t) = malloc;
static void (*wfFreeFunc)(void *) = free;

void wfSetMemoryAllocator(void * (*allocfn)(size_t),void (*freefn)(void *))
{
 wfAllocFunc = allocfn;
 wfFreeFunc = freefn;
}

void *wfAlloc(size_t n)
{
 void *p;
 p = (*wfAllocFunc)(n);
 if (!p) fprintf(stderr,"WF ERROR: alloc failed\n");
 return p;
}

void wfFree(void *p)
{
 (*wfFreeFunc)(p);
}
