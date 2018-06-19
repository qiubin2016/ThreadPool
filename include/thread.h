#ifndef __THREAD_H__
#define __THREAD_H__

#include "Include.h"
#include <pthread.h>

#define TSK_DEF_STACK_SIZE				(16384*2)		///16k

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef THREAD_HANDLE
  #define THREAD_HANDLE pthread_t
#endif


int  ThreadCreate(THREAD_HANDLE * pthread, void(*pStartAddress)(void* p), void* pParameter, int nPriority, uint32_t dwStackSize);
int  ThreadDestroy(THREAD_HANDLE hthread);
void ThreadExit(void);
int  ThreadGetId(void);
void ThreadSetName(const char* name);

#ifdef __cplusplus
}
#endif

#endif
