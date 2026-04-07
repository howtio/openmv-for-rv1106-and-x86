#ifndef __OMV_PROFILER_H__
#define __OMV_PROFILER_H__

// 骗过编译器，把所有性能测试相关的宏变成空操作 (不做任何事)
#define OMV_PROFILE_START(name) 
#define OMV_PROFILE_END(name)   
#define OMV_PROFILE_ALLOC()     

#endif // __OMV_PROFILER_H__
