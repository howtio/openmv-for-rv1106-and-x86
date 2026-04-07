#ifndef __MP_UTILS_H__
#define __MP_UTILS_H__
#include <stdio.h>

// 当算法出错时，原版会抛出 Python 异常，我们直接让它打印出 C 语言错误信息
#define mp_raise_msg(type, msg) printf("Error: %s\n", msg)

#endif