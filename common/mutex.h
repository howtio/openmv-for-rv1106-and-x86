#ifndef __MUTEX_H__
#define __MUTEX_H__

// 假装有一个锁的结构体
typedef int mutex_t; 

// 把锁的操作直接变成空动作（单线程下根本不需要锁）
#define mutex_init(m)   
#define mutex_lock(m, t) (1)
#define mutex_unlock(m) 

#endif