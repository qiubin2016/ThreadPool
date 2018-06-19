#ifndef __MUTEX_SEMAPHORE_H__
#define __MUTEX_SEMAPHORE_H__

#include <semaphore.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "comdef.h"

#ifndef MUTEX_HANDLE
#define MUTEX_HANDLE   		pthread_mutex_t
#endif


#ifndef SEMAPHORE_HANDLE
#define SEMAPHORE_HANDLE		sem_t
#endif

#ifndef COND_HANDLE
#define COND_HANDLE   		pthread_cond_t
#endif

enum mutex_type
{
	MUTEX_FAST = 0,
	MUTEX_RECURSIVE,
};

#ifdef __cplusplus
extern "C"
{
#endif

int SemaphoreCreate(SEMAPHORE_HANDLE* Semaphore, uint32_t dwInitialCount);
int SemaphoreDestroy(SEMAPHORE_HANDLE* Semaphore);
int SemaphorePend(SEMAPHORE_HANDLE* Semaphore);
int SemaphorePost(SEMAPHORE_HANDLE* Semaphore);

int MutexCreate(MUTEX_HANDLE* hMutex, int nType);
int MutexDestroy(MUTEX_HANDLE* hMutex);
int MutexEnter(MUTEX_HANDLE* hMutex);
int MutexLeave(MUTEX_HANDLE* hMutex);

int CondCreate(COND_HANDLE* Cond);
int CondDestroy(COND_HANDLE* Cond);
int CondWait(COND_HANDLE* Cond, MUTEX_HANDLE* Mutex);
int CondTimedWait(COND_HANDLE* Cond, MUTEX_HANDLE* Mutex, const struct timespec *Tsptr);
int CondSignal(COND_HANDLE* Cond);
int CondBroadCast(COND_HANDLE* Cond);

#ifdef __cplusplus
}
#endif
#endif
