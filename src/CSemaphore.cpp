#include "CSemaphore.h"

CSemaphore::CSemaphore(uint32_t dwInitialCount)
{
	SemaphoreCreate(&m_Semaphore, dwInitialCount);
}

CSemaphore::~CSemaphore()
{
	SemaphoreDestroy(&m_Semaphore);
}

int CSemaphore::Pend()
{
	return SemaphorePend(&m_Semaphore);
}

int CSemaphore::Post()
{
	return SemaphorePost(&m_Semaphore);
}