#ifndef _CMUTEX_H__
#define _CMUTEX_H__

#include "mutexsemaphore.h"

class CMutex
{
public:
	CMutex(int nType = MUTEX_FAST);
	~CMutex();
	int Enter();
	int Leave();
	MUTEX_HANDLE& GetMutex();
private:
	MUTEX_HANDLE m_hMutex;
};


#endif
