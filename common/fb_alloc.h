#ifndef __FB_ALLOC_H__
#define __FB_ALLOC_H__
#include <stdlib.h>
#include <stdio.h>

#define FB_ALLOC_NO_HINT 0
#define FB_ALLOC_PREFER_SIZE 1
#define FB_ALLOC_PREFER_SPEED 2

#define fb_alloc(size, hints)  malloc(size)
#define fb_alloc0(size, hints) calloc(1, size)
#define fb_free()              
#define fb_free_all()          

// 1. 强行给它 4GB 虚拟可用额度！堵住它的嘴！
#define fb_avail() ((size_t)0xFFFFFFFF) 

// 2. 强行分配 100MB 真实缓存池！
#define fb_alloc_all(size_ptr, hints) (*(size_ptr) = 1024*1024*100, malloc(1024*1024*100))

// 3. 【终极追踪】打印出到底是哪个文件的哪一行代码引发了失败！
#define fb_alloc_fail() do { printf("Fatal: 内存自检失败！被拦截在文件: %s, 行号: %d\n", __FILE__, __LINE__); exit(1); } while(0)

#endif /* __FB_ALLOC_H__ */
