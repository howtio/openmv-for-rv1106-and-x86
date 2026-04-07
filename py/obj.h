#ifndef _PY_OBJ_H
#define _PY_OBJ_H

#include <stdlib.h>

// 架空万能对象和字符串对象
typedef void* mp_obj_t;
typedef const char* mp_rom_error_text_t;

#define mp_const_none NULL
#define mp_const_true ((void*)1)
#define mp_const_false ((void*)0)

// 强行把 MicroPython 内存分配替换成标准 Linux 库！(消除 array.c 的警告)
#define m_malloc(size)       malloc(size)
#define m_free(ptr)          free(ptr)
#define m_realloc(ptr, size) realloc(ptr, size)

// 架空大写的栈溢出检查宏
#define MP_STACK_CHECK()

#endif
