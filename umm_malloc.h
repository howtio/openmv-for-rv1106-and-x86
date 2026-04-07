#ifndef _UMM_MALLOC_H
#define _UMM_MALLOC_H

#include <stdlib.h>

// 把单片机专用的碎片整理分配器，全部强行映射为 Linux 标准库！
#define umm_malloc(size)       malloc(size)
#define umm_free(ptr)          free(ptr)
#define umm_calloc(num, size)  calloc(num, size)
#define umm_realloc(ptr, size) realloc(ptr, size)

#endif
