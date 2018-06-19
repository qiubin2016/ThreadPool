/*
 * CCondition.h
 *
 *  Created on: 2017年2月16日
 *      Author: qb
 */

#ifndef CCONDITION_H_
#define CCONDITION_H_

#include "CMutex.h"

class CCondition
{
public:
	CCondition();
	virtual ~CCondition();

	void Wait(CMutex& Mutex);
	void Wait(CMutex& Mutex, unsigned int uiMs);
	void Signal();
	void BroadCast();
protected:

private:
	void maketimeout(struct timespec *pTsp, unsigned int ulMs);
private:
	mutable COND_HANDLE m_Cond;
};



#endif /* CCONDITION_H_ */
