#ifndef __OMV_COMMON_H__
#define __OMV_COMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h> // 引入标准库，确保能找到 malloc/free

#define OMV_MIN(a,b) \
    ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define OMV_MAX(a,b) \
    ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

#define OMV_ATTR_ALIGNED(x) __attribute__((aligned(x)))
#define OMV_ATTR_ALWAYS_INLINE inline __attribute__((always_inline))
#define OMV_ATTR_WEAK __attribute__((weak))
#define OMV_ATTR_SECTION(x) 

#define fast_roundf(x) ((int)roundf(x))
#define fast_floorf(x) ((int)floorf(x))
#define fast_ceilf(x)  ((int)ceilf(x))

#define fast_atan2f(y, x) atan2f(y, x)
#define fast_sqrtf(x) sqrtf(x)

#define OMV_CACHE_LINE_SIZE 32
#define OMV_ALIGN_TO(size, align) \
    ((__typeof__(size))(((uintptr_t)(size) + ((uintptr_t)(align) - 1)) & ~((uintptr_t)(align) - 1)))

#ifndef __USAT
#define __USAT(val, sat) ((val) < 0 ? 0 : ((val) > ((1U << (sat)) - 1) ? ((1U << (sat)) - 1) : (val)))
#endif

#ifndef __SSAT
#define __SSAT(val, sat) ((val) < -(1 << ((sat) - 1)) ? -(1 << ((sat) - 1)) : ((val) > ((1 << ((sat) - 1)) - 1) ? ((1 << ((sat) - 1)) - 1) : (val)))
#endif

#ifndef __PKHBT
#define __PKHBT(op1, op2, shift) ((((uint32_t)(op1)) & 0x0000FFFF) | ((((uint32_t)(op2)) << (shift)) & 0xFFFF0000))
#endif

#ifndef __SMLAD
#define __SMLAD(op1, op2, op3) \
    ((int32_t)((int16_t)(((uint32_t)(op1)) & 0xFFFF)) * (int32_t)((int16_t)(((uint32_t)(op2)) & 0xFFFF)) + \
     (int32_t)((int16_t)(((uint32_t)(op1)) >> 16)) * (int32_t)((int16_t)(((uint32_t)(op2)) >> 16)) + \
     (int32_t)(op3))
#endif

// =========================================================
// 【终极替身】替换 MicroPython 的内存函数
// 注意：不要带括号！这样才能作为函数指针无缝传递给数组！
// =========================================================
#define m_malloc malloc
#define m_free free
#define m_realloc realloc

#endif // __OMV_COMMON_H__
