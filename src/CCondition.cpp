/*
 * CCondition.cpp
 *
 *  Created on: 2017年2月16日
 *      Author: qb
 */


#include "CCondition.h"

CCondition::CCondition()
{
	CondCreate(&m_Cond);
}

CCondition::~CCondition()
{
	CondDestroy(&m_Cond);
}

void CCondition::Wait(CMutex &Mutex)
{
	CondWait(&m_Cond, &Mutex.GetMutex());
}

void CCondition::Wait(CMutex &Mutex, unsigned int uiMs)
{
	struct timespec Tsp;

	maketimeout(&Tsp, uiMs);
	CondTimedWait(&m_Cond, &Mutex.GetMutex(), &Tsp);
}

void CCondition::Signal()
{
	CondSignal(&m_Cond);
}

void CCondition::BroadCast()
{
	CondBroadCast(&m_Cond);
}

void CCondition::maketimeout(struct timespec *pTsp, unsigned int ulMs)
{
	struct timeval now;

	//get the current time
	gettimeofday(&now, NULL);
//	gettimeofday(&now);
	pTsp->tv_sec = now.tv_sec;
	//usec to nsec
	pTsp->tv_nsec = now.tv_usec * 1000;
	//add the offset to get timeout valude
	pTsp->tv_nsec += (ulMs % 1000) * 1000000L;
	pTsp->tv_sec += (ulMs / 1000) + (pTsp->tv_nsec / 1000000000L);
	pTsp->tv_nsec = pTsp->tv_nsec % 1000000000L;
}
