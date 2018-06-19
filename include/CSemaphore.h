#ifndef __CSEMAPHORE_H__
#define __CSEMAPHORE_H__

#include "mutexsemaphore.h"

class CSemaphore
{
public:
	CSemaphore(uint32_t dwInitialCount = 0);
	~CSemaphore();
	int Pend();
	int Post();
private:
	SEMAPHORE_HANDLE m_Semaphore;
};


#endif