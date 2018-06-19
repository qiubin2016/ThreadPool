#ifndef CWORKERTHREAD_H_
#define CWORKERTHREAD_H_

#include "CThreadA.h"
#include "CJob.h"
#include "CMutex.h"
#include "CCondition.h"
//#include "CThreadPool.h"

class CThreadPool;
class CWorkerThread: public CThreadA
{
    private:
        CThreadPool *m_ThreadPool;
        CJob *m_Job;
        void *m_JobData;
        CMutex m_VarMutex;
        bool m_IsEnd;
    protected:
    public:
        CCondition m_JobCond;
        CMutex m_WorkMutex;
        CWorkerThread();
        virtual ~CWorkerThread();
        void Run();
        void SetJob(CJob *job, void *jobdata);
        CJob *GetJob(void) {return m_Job;}
        void SetThreadPool(CThreadPool *thrpool);
        CThreadPool *GetThreadPool(void) {return m_ThreadPool;}
	void Stop(void);
};

#endif
