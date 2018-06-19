
//#include "CWorkerThread.h"
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "BuildConfig.h"
#include "misc.h"
#include "Cpp.h"
#include "CThreadPool.h"


CThreadPool::CThreadPool()
{
    m_MaxNum = THREAD_MAX_NUM;
    m_AvailLow = THREAD_AVAIL_LOW;
    m_InitNum = m_AvailNum = THREAD_INIT_NUM ;
    m_AvailHigh = THREAD_AVAIL_HIGH;
    m_BusyList.clear();
    m_IdleList.clear();
    for (int i = 0; i < (int)m_InitNum; i++)
    {
        CWorkerThread *thr = new CWorkerThread();
        thr->SetThreadPool(this);
        AppendToIdleList(thr);
        thr->Start();
    }
}



CThreadPool::CThreadPool(int initnum)
{
    int Num = 0.2 * THREAD_MAX_NUM;
    if (initnum > 0 && initnum <= THREAD_MAX_NUM)
    {
        m_MaxNum   = THREAD_MAX_NUM;
        m_AvailLow = initnum - Num > 0 ? initnum - Num : THREAD_AVAIL_LOW;
        m_InitNum = m_AvailNum = initnum ;
        if ((initnum + Num) >= THREAD_MAX_NUM)
        {
            m_AvailHigh = THREAD_AVAIL_HIGH;
        }
        else
        {
            m_AvailHigh = initnum + Num;
        }
    }
    else
    {
        m_MaxNum = THREAD_MAX_NUM;
        m_AvailLow = THREAD_AVAIL_LOW;
        m_InitNum = m_AvailNum = THREAD_INIT_NUM ;
        m_AvailHigh = THREAD_AVAIL_HIGH;
    }
	m_MaxNum = 4;
	m_aVailLow = 1;
	m_InitNum = 2;
	m_aVailHigh = 3;
	DBG_POOL("max:%d,low:%d,init:%d,avalid:%d,high:%d\n", m_MaxNum,m_AvailLow,m_InitNum,m_AvailNum,m_AvailHigh);
    m_BusyList.clear();
    m_IdleList.clear();
    //string Name;
    for (int i = 0; i < (int)m_InitNum; i++)
    {
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++\n");
        CWorkerThread *thr = new CWorkerThread();
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++\n");
        AppendToIdleList(thr);
        thr->SetThreadPool(this);
        thr->Start();       //begin the thread,the thread wait for job
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++\n");
    }
}



CThreadPool::~CThreadPool()

{
    TerminateAll();
}



void CThreadPool::TerminateAll()
{
	DBG_POOL("thread_list_size:%d\n", m_ThreadList.size());
    for (int i = 0; i < (int)m_ThreadList.size(); i++)
    {
        DBG_POOL("\n");
        CWorkerThread *thr = m_ThreadList[i];
        thr->Stop();
        delete thr;
        DBG_POOL("\n");
    }
    return;
}

CWorkerThread *CThreadPool::GetIdleThread(void)

{
    m_IdleMutex.Enter();
    while (m_IdleList.size() == 0 )
    { 
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++idle:%u,busy:%u,thread:%u,time:%u\n",
                                                                                                     m_IdleList.size(),
                                                                                                     m_BusyList.size(),
                                                                                                     m_ThreadList.size(), SystemGetMSCount());
        m_IdleCond.Wait(m_IdleMutex);    //空闲线程列表为空,挂起等待。
    }
    DBG_POOL("++++++++++++++++++++++++++++++++++++++++++idle:%u,busy:%u,thread:%u,time:%u\n",
                                                                                                     m_IdleList.size(),
                                                                                                     m_BusyList.size(),
                                                                                                     m_ThreadList.size(), SystemGetMSCount());
    //m_IdleMutex.Enter();
    if (m_IdleList.size() > 0 )
    {
        CWorkerThread *thr = (CWorkerThread *)m_IdleList.front();
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++Get Idle thread %d\n", thr->GetThreadID());
        m_IdleMutex.Leave();
        return thr;
    }
    DBG_POOL("++++++++++++++++++++++++++++++++++++++++++\n");
    m_IdleMutex.Leave();
    return NULL;
}



//add an idle thread to idle list

void CThreadPool::AppendToIdleList(CWorkerThread *jobthread)

{
    m_IdleMutex.Enter();
    m_IdleList.push_back(jobthread);
    m_ThreadList.push_back(jobthread);
    m_IdleMutex.Leave();
}



//move and idle thread to busy thread

void CThreadPool::MoveToBusyList(CWorkerThread *idlethread)

{
    m_BusyMutex.Enter();
    m_BusyList.push_back(idlethread);
    m_AvailNum--;
    m_BusyMutex.Leave();
    m_IdleMutex.Enter();
    std::vector<CWorkerThread *>::iterator pos;
    pos = find(m_IdleList.begin(), m_IdleList.end(), idlethread);
    if (pos != m_IdleList.end())
    { m_IdleList.erase(pos); }
    m_IdleMutex.Leave();
}



void CThreadPool::MoveToIdleList(CWorkerThread *busythread)

{
    m_IdleMutex.Enter();
    m_IdleList.push_back(busythread);
    m_AvailNum++;   //更新目前线程池中实际存在的线程的个数
    m_IdleMutex.Leave();
    m_BusyMutex.Enter();
    std::vector<CWorkerThread *>::iterator pos;
    pos = find(m_BusyList.begin(), m_BusyList.end(), busythread);
    if (pos != m_BusyList.end())
    { 
    	m_BusyList.erase(pos); 
    }
    m_BusyMutex.Leave();
    //线程从忙碌切换到空闲,通知空闲队列,通知因无空闲线程而挂起的操作
    m_IdleCond.Signal();    
    m_MaxNumCond.Signal();
}



//create num idle thread and put them to idlelist

void CThreadPool::CreateIdleThread(int num)

{
    for (int i = 0; i < num; i++)
    {
        CWorkerThread *thr = new CWorkerThread();
        thr->SetThreadPool(this);
        AppendToIdleList(thr);
        m_VarMutex.Enter();
        m_AvailNum++;
        m_VarMutex.Leave();
        thr->Start();       //begin the thread,the thread wait for job
    }
}



void CThreadPool::DeleteIdleThread(int num)

{
    DBG_POOL("++++++++++++++++++++++++++++++++++++++++++Enter into CThreadPool::DeleteIdleThread\n");
    m_IdleMutex.Enter();
    DBG_POOL("++++++++++++++++++++++++++++++++++++++++++Delete Num is %d\n", num);
    for (int i = 0; i < num; i++)
    {
        CWorkerThread *thr;
        if (m_IdleList.size() > 0 )
        {
            thr = (CWorkerThread *)m_IdleList.front();
            DBG_POOL("++++++++++++++++++++++++++++++++++++++++++Get Idle thread %d\n", thr->GetThreadID());
            //销毁线程
            thr->Stop();
            delete thr;
        }
        std::vector<CWorkerThread *>::iterator pos;
        pos = find(m_IdleList.begin(), m_IdleList.end(), thr);
        if (pos != m_IdleList.end())
        { 
            DBG_POOL("++++++++++++++++++++++++++++++++++++++++++@@@@@@@@@@@@@@@Get Idle thread %d\n", thr->GetThreadID());
            m_IdleList.erase(pos); 
            if (m_AvailNum)
            {
                m_AvailNum--;    //更新目前线程池中实际存在的线程的个数
            }
        }
        pos = find(m_ThreadList.begin(), m_ThreadList.end(), thr);
        if (pos != m_ThreadList.end())
        { 
            DBG_POOL("++++++++++++++++++++++++++++++++++++++++++@@@@@@@@@@@@@@@Get Idle thread %d\n", thr->GetThreadID());
            m_ThreadList.erase(pos); 
        }
 
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++The idle thread available num:%d \n", m_AvailNum);
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++The idlelist              num:%u \n", m_IdleList.size());
    }
    m_IdleMutex.Leave();
}

/*
    线程池在接受到新的任务之后，线程池首先要检查是否有足够的空闲池可用。
检查分为三个步骤：
(1)检查当前处于忙碌状态的线程是否达到了设定的最大值m_MaxNum，如果达到了，
表明目前没有空闲线程可用，而且也不能创建新的线程，因此必须等待直到有
线程执行完毕返回到空闲队列中。
(2)如果当前的空闲线程数目小于我们设定的最小的空闲数目m_AvailLow，则我们必
须创建新的线程，默认情况下，创建后的线程数目应该为m_InitNum，因此创建的
线程数目应该为( 当前空闲线程数与m_InitNum);但是有一种特殊情况必须考虑，就
是现有的线程总数加上创建后的线程数可能超过m_MaxNum，因此我们必须对线程
的创建区别对待。
if(GetAllNum()+m_InitNum-m_IdleList.size() < m_MaxNum)
    CreateIdleThread(m_InitNum-m_IdleList.size());
else
    CreateIdleThread(m_MaxNum-GetAllNum());
    如果创建后总数不超过m_MaxNum，则创建后的线程为m_InitNum；如果超过了，则只
    创建( m_MaxNum-当前线程总数 )个。
(3)调用GetIdleThread方法查找空闲线程。如果当前没有空闲线程，则挂起；否则将任
务指派给该线程，同时将其移入忙碌队列。
当线程执行完毕后，其会调用MoveToIdleList方法移入空闲链表中，其中还调用
m_IdleCond.Signal()方法，唤醒GetIdleThread()中可能阻塞的线程。
*/
void CThreadPool::Run(CJob *job, void *jobdata)

{
    assert(job != NULL);
    //if the busy thread num adds to m_MaxNum,so we should wait
    m_BusyMutex.Enter();
    if (GetBusyNum() == (int)m_MaxNum)
    { 
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++idle:%u,busy:%u,thread:%u,time:%u\n",
                                                                                                     m_IdleList.size(),
                                                                                                     m_BusyList.size(),
                                                                                                     m_ThreadList.size(), SystemGetMSCount());
        m_MaxNumCond.Wait(m_BusyMutex);    //当前线程数量达到最大且均忙碌,挂起等待
    }
    m_BusyMutex.Leave();
    DBG_POOL("++++++++++++++++++++++++++++++++++++++++++idle:%u,busy:%u,thread:%u,time:%u\n",
                                                                                                     m_IdleList.size(),
                                                                                                     m_BusyList.size(),
                                                                                                     m_ThreadList.size(), SystemGetMSCount());

    CWorkerThread  *idlethr = GetIdleThread();
    if (idlethr != NULL)
    {
        idlethr->m_WorkMutex.Enter();
        MoveToBusyList(idlethr);
        idlethr->SetThreadPool(this);
        job->SetWorkThread(idlethr);
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++Job is set to thread %d \n", idlethr->GetThreadID());
        idlethr->SetJob(job, jobdata);
        if (m_IdleList.size() < m_AvailLow)    //空闲线程数低于最低限值
        {
            if (GetAllNum() + m_InitNum - m_IdleList.size() < m_MaxNum )
            {
                DBG_POOL("++++++++++++++++++++++++++++++++++++++++++idle:%u,busy:%u,thread:%u,time:%u,create:%u\n",
                         m_IdleList.size(), m_BusyList.size(), m_ThreadList.size(), SystemGetMSCount(), m_InitNum - (unsigned int)m_IdleList.size());
                CreateIdleThread(m_InitNum - m_IdleList.size());
            }
            else
            {
                DBG_POOL("++++++++++++++++++++++++++++++++++++++++++idle:%u,busy:%u,thread:%u,time:%u,create:%u\n",
                         m_IdleList.size(), m_BusyList.size(), m_ThreadList.size(), SystemGetMSCount(), m_MaxNum - (unsigned int)GetAllNum());
                CreateIdleThread(m_MaxNum - GetAllNum());
            }
        }
        DBG_POOL("++++++++++++++++++++++++++++++++++++++++++idle:%u,busy:%u,thread:%u\n", m_IdleList.size(), m_BusyList.size(), m_ThreadList.size());
    }
}
