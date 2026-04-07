#ifndef _PY_RUNTIME_H
#define _PY_RUNTIME_H

#include <stdio.h>
#include <stdlib.h>

// 1. 引入我们之前造好的假对象头文件
#include "obj.h"

// 2. 暴力拦截所有 MicroPython 的抛出异常操作！
#define nlr_raise(val) do { printf("Fatal Error: nlr_raise triggered!\n"); exit(1); } while(0)

#define mp_raise_ValueError(msg) do { printf("ValueError: %s\n", msg); exit(1); } while(0)

#define mp_raise_msg_varg(type, fmt, ...) do { printf("MP_ERROR: " fmt "\n", ##__VA_ARGS__); exit(1); } while(0)

#define mp_raise_TypeError(msg) do { printf("TypeError: %s\n", msg); exit(1); } while(0)

#endif
