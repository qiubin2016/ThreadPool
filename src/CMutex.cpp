#include "CMutex.h"

CMutex::CMutex(int nType)
{
	MutexCreate(&m_hMutex, nType);
}

CMutex::~CMutex()
{
	MutexDestroy(&m_hMutex);
}

int CMutex::Enter()
{
	return MutexEnter(&m_hMutex) == 0? 0:-1;
}

int CMutex::Leave()
{
	return MutexLeave(&m_hMutex) == 0? 0: -1;
}

MUTEX_HANDLE& CMutex::GetMutex()
{
	return m_hMutex;
}
