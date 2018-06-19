#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H

#include <vector>
#include "CJob.h"
#include "CMutex.h"
#include "CCondition.h"
#include "CWorkerThread.h"

#define THREAD_MAX_NUM    10
#define THREAD_AVAIL_LOW  2
#define THREAD_AVAIL_HIGH 8
#define THREAD_INIT_NUM   3

class CThreadPool
{
    friend class CWorkerThread;

private:
    uint32_t m_MaxNum;   //the max thread num that can create at the same time
    uint32_t m_AvailLow; //The min num of idle thread that shoule kept
    uint32_t m_AvailHigh;    //The max num of idle thread that kept at the same time
    uint32_t m_AvailNum; //the normal thread num of idle num;
    uint32_t m_InitNum;  //Normal thread num;

protected:
    CWorkerThread *GetIdleThread(void);
    void    AppendToIdleList(CWorkerThread *jobthread);
    void    MoveToBusyList(CWorkerThread *idlethread);
    void    MoveToIdleList(CWorkerThread *busythread);
    void    DeleteIdleThread(int num);
    void    CreateIdleThread(int num);

public:
    CMutex m_BusyMutex;    //when visit busy list,use m_BusyMutex to lock and unlock
    CMutex m_IdleMutex;    //when visit idle list,use m_IdleMutex to lock and unlock
    CMutex m_JobMutex; //when visit job list,use m_JobMutex to lock and unlock
    CMutex m_VarMutex;
 CMutex m_MaxNumMutex;
    CCondition       m_BusyCond; //m_BusyCond is used to sync busy thread list
    CCondition       m_IdleCond; //m_IdleCond is used to sync idle thread list
    CCondition       m_IdleJobCond;  //m_JobCond is used to sync job list
    CCondition       m_MaxNumCond;
    std::vector<CWorkerThread *>   m_ThreadList;
    std::vector<CWorkerThread *>   m_BusyList;     //Thread List
    std::vector<CWorkerThread *>   m_IdleList; //Idle List
    CThreadPool();
    CThreadPool(int initnum);
    virtual ~CThreadPool();
    void    SetMaxNum(int maxnum) {m_MaxNum = maxnum;}
    int     GetMaxNum(void) {return m_MaxNum;}
    void    SetAvailLowNum(int minnum) {m_AvailLow = minnum;}
    int     GetAvailLowNum(void) {return m_AvailLow;}
    void    SetAvailHighNum(int highnum) {m_AvailHigh = highnum;}
    int     GetAvailHighNum(void) {return m_AvailHigh;}
    int     GetActualAvailNum(void) {return m_AvailNum;}
    int     GetAllNum(void) {return m_ThreadList.size();}
    int     GetBusyNum(void) {return m_BusyList.size();}
    void    SetInitNum(int initnum) {m_InitNum = initnum;}
    int     GetInitNum(void) {return m_InitNum;}
    void    TerminateAll(void);
    void    Run(CJob *job, void *jobdata);
};
#endif // CTHREADPOOL_H
