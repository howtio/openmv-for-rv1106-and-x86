#ifndef _IMLIB_CONFIG_H
#define _IMLIB_CONFIG_H

// 1. 开启我们要测试的核心功能：找圆算法！
#define IMLIB_ENABLE_FIND_CIRCLES

// 2. 屏蔽掉一些默认开启的单片机硬件依赖（防止后续连环爆炸）
#define OMV_HARDWARE_JPEG 0

// 后续如果还要测找色块、找直线，就往这里加 #define IMLIB_ENABLE_FIND_BLOBS 等等...

#endif
