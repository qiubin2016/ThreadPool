#include "BuildConfig.h"
#include "misc.h"
#include "CThreadPool.h"
#include "CWorkerThread.h"


CWorkerThread::CWorkerThread()
{
    DBG_POOL("++++++++++++++++++++++++++++++++++++++++++\n");
    m_Job = NULL;
    m_JobData = NULL;
    m_ThreadPool = NULL;
    m_IsEnd = false;
    DBG_POOL("++++++++++++++++++++++++++++++++++++++++++\n");
}
CWorkerThread::~CWorkerThread()
{
    if (NULL != m_Job)
    {
//        delete m_Job;
    }
    if (NULL != m_ThreadPool)
    {
//        delete m_ThreadPool;
    }
}
void CWorkerThread::Run()
{
    SetThreadState(THREAD_RUNNING);
    for ( ;; )
    {
        DBG_POOL("----------------------\n");
        while (m_Job == NULL)
        {
            DBG_POOL("----------------------id:%d,idle:%u,busy:%u,thread:%u,time:%u\n", GetThreadID(),
                                                                                                     m_ThreadPool->m_IdleList.size(),
                                                                                                     m_ThreadPool->m_BusyList.size(),
                                                                                                     m_ThreadPool->m_ThreadList.size(), SystemGetMSCount());
            m_JobCond.Wait(m_VarMutex);    //挂起等待任务到来
            Exit();
        }
        DBG_POOL("----------------------id:%d,idle:%u,busy:%u,thread:%u,time:%u\n", GetThreadID(),
                                                                                                     m_ThreadPool->m_IdleList.size(),
                                                                                                     m_ThreadPool->m_BusyList.size(),
                                                                                                     m_ThreadPool->m_ThreadList.size(), SystemGetMSCount());
        m_Job->Run(m_JobData);    //ִ执行实际的操作
        DBG_POOL("----------------------id:%d,idle:%u,busy:%u,thread:%u,time:%u\n", GetThreadID(),
                                                                                                     m_ThreadPool->m_IdleList.size(),
                                                                                                     m_ThreadPool->m_BusyList.size(),
                                                                                                     m_ThreadPool->m_ThreadList.size(), SystemGetMSCount());
        m_Job->SetWorkThread(NULL);
        m_Job = NULL;
        m_ThreadPool->MoveToIdleList(this);    //将工作线程添加到空闲队列
        DBG_POOL("----------------------id:%d,idle:%u,busy:%u,thread:%u,time:%u\n", GetThreadID(),
                                                                                                     m_ThreadPool->m_IdleList.size(),
                                                                                                     m_ThreadPool->m_BusyList.size(),
                                                                                                     m_ThreadPool->m_ThreadList.size(), SystemGetMSCount());
        //空闲线程数量大于当前线程池中所允许的空闲的线程的最大数目,说明负荷轻
        if ((int)m_ThreadPool->m_IdleList.size() > m_ThreadPool->GetAvailHighNum())
        {
            DBG_POOL("----------------------id:%d,idle:%u,busy:%u,thread:%u,time:%u\n", GetThreadID(),
                                                                                                     m_ThreadPool->m_IdleList.size(),
                                                                                                     m_ThreadPool->m_BusyList.size(),
                                                                                                     m_ThreadPool->m_ThreadList.size(), SystemGetMSCount());
                        //ֻ只保留初始创建时线程池中的线程的个数
            m_ThreadPool->DeleteIdleThread(m_ThreadPool->m_IdleList.size() -m_ThreadPool->GetInitNum());
        }
        m_WorkMutex.Leave();
        DBG_POOL("----------------------id:%d,idle:%u,busy:%u,thread:%u,time:%u\n", GetThreadID(),
                                                                                                     m_ThreadPool->m_IdleList.size(),
                                                                                                     m_ThreadPool->m_BusyList.size(),
                                                                                                     m_ThreadPool->m_ThreadList.size(), SystemGetMSCount());
    }
}
void CWorkerThread::SetJob(CJob *job, void *jobdata)
{
    m_VarMutex.Enter();
    m_Job = job;
    m_JobData = jobdata;
    job->SetWorkThread(this);
    m_VarMutex.Leave();
    m_JobCond.Signal();    //任务到来,通知当前线程开始处理任务
}
void CWorkerThread::SetThreadPool(CThreadPool *thrpool)
{
    m_VarMutex.Enter();
    m_ThreadPool = thrpool;
    m_VarMutex.Leave();
}

void CWorkerThread::Stop(void)
{
    Terminate();
    m_JobCond.Signal();    //唤醒线程
    Join();
}
