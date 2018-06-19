#include "mutexsemaphore.h"

int SemaphoreCreate(SEMAPHORE_HANDLE* Semaphore, uint32_t dwInitialCount)
{
	if(sem_init((sem_t*)Semaphore, 0, dwInitialCount) != 0)
	{
		printf("sem_init failed!\n");
		return -1;
	}
	return 0;
}

int SemaphoreDestroy(SEMAPHORE_HANDLE* Semaphore)
{
	if(sem_destroy((sem_t*)Semaphore) == -1)
	{
		return -1;
	}
	return 0;
}

int SemaphorePend(SEMAPHORE_HANDLE* Semaphore)
{
	return sem_wait((sem_t*)Semaphore);
}

int SemaphorePost(SEMAPHORE_HANDLE* Semaphore)
{
	return sem_post((sem_t*)Semaphore);
}

int MutexCreate(MUTEX_HANDLE* hMutex, int nType)
{
	int Ret;
	pthread_mutexattr_t attr;

	switch(nType)
	{
		case MUTEX_RECURSIVE:
		{
			if((Ret = pthread_mutexattr_init(&attr)) != 0)
			{
				return -1;
			}

			if((Ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP)) != 0)
			{
				return -1;
			}

			if((Ret = pthread_mutex_init((pthread_mutex_t*)hMutex, &attr)) != 0)
			{
				return -1;
			}
			break;
		}
		case MUTEX_FAST:
		default:
		{
			Ret = pthread_mutex_init((pthread_mutex_t*)hMutex, NULL);
			if(Ret != 0)
			{
				return -1;
			}
			break;
		}
	}

	return 0;
}


int MutexDestroy(MUTEX_HANDLE* hMutex)
{
	pthread_mutex_destroy((pthread_mutex_t*)hMutex);
	return 0;
}

int MutexEnter(MUTEX_HANDLE* hMutex)
{
	return -pthread_mutex_lock((pthread_mutex_t*)hMutex);
}

int MutexLeave(MUTEX_HANDLE* hMutex)
{
	return -pthread_mutex_unlock((pthread_mutex_t*)hMutex);
}

int CondCreate(COND_HANDLE* Cond)
{
	return pthread_cond_init(Cond, NULL);
}

int CondDestroy(COND_HANDLE* Cond)
{
	return pthread_cond_destroy(Cond);
}

int CondWait(COND_HANDLE* Cond, MUTEX_HANDLE* Mutex)
{
	return pthread_cond_wait(Cond, Mutex);
}

int CondTimedWait(COND_HANDLE* Cond, MUTEX_HANDLE* Mutex, const struct timespec *Tsptr)
{
	return pthread_cond_timedwait(Cond, Mutex, Tsptr);
}

int CondSignal(COND_HANDLE* Cond)
{
	return pthread_cond_signal(Cond);
}

int CondBroadCast(COND_HANDLE* Cond)
{
	return pthread_cond_broadcast(Cond);
}
