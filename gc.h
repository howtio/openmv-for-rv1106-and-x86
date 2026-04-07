#ifndef __GC_H__
#define __GC_H__

#include <stdlib.h>

// 彻底架空 MicroPython 的垃圾回收器 (GC)
// 把所有带有 gc_ 前缀的内存申请，全部扔给 Linux 标准 C 库！
#define gc_alloc(size, has_ptr) malloc(size)
#define gc_alloc0(size, has_ptr) calloc(1, size)
#define gc_free(ptr) free(ptr)

#endif
