#include <cstring>
#include "BuildConfig.h"
#include "Cpp.h"
#include "CThreadA.h"

int CThreadA::m_CreateCount = 0;

CThreadA::CThreadA():m_ThreadSemaphore(0)
{
    ValInit(FALSE, FALSE);
}

CThreadA::CThreadA(BOOL createsuspended, BOOL detach):m_ThreadSemaphore(0)
{
    ValInit(createsuspended, detach);
}

CThreadA::~CThreadA()
{

}

void CThreadA::ValInit(BOOL createsuspended, BOOL detach)
{
    m_Detach = detach;
    m_CreateSuspended = createsuspended;
    m_ErrCode = 0;
    m_ThreadID = -1;
    m_Detach = FALSE;
    m_CreateSuspended = FALSE;
    m_CreateCount++;
    string sStr = string(__func__);
    sStr += Int2String(m_CreateCount);
    m_ThreadName = sStr;
    m_ThreadState = THREAD_SUSPENDED;
    m_Poriority = PORIORITY_DEF;
    m_StackSize = STACK_SIZE_DEF;
    m_hThread = 0;
}

void CThreadA::ThreadFunction(void *pArg)
{
    CThreadA* pThread = (CThreadA* )pArg;

    pThread->m_ThreadID = ThreadGetId();
    DBG_POOL("id = %d\n", pThread->m_ThreadID);

    ThreadSetName(pThread->m_ThreadName.c_str());
    DBG_POOL("name = %s\n",pThread->m_ThreadName.c_str());

    if (pThread->m_CreateSuspended)    //线程启动后是否自动挂起
    {
        pThread->m_ThreadSemaphore.Pend();    //等待唤醒
    }
    pThread->Run();

    DBG_POOL("Leave name =%s, id=%d, handle=0x%x,stack=0x%x\n",
    		pThread->m_ThreadName.c_str(),pThread->m_ThreadID, (int)pThread->m_hThread, pThread->m_StackSize);

    //if(pThread->m_bWaitThreadExit)
    //{

    	//pThread->m_bWaitThreadExit = FALSE;
    	//pThread->m_desSemaphore.Post();
    //}
    DBG_POOL("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    pThread->m_ThreadID = -1;


    //pThread->m_cSemaphore.Post();

    ThreadExit();
}

BOOL CThreadA::Terminate(void)
{
    m_ThreadState = THREAD_STOPED;    //改变线程状态
    m_ThreadID = -1;
    return TRUE;
}

BOOL CThreadA::Start(void)
{
    BOOL bRet;
    bRet = ThreadCreate(&m_hThread, (void(*)(void*))ThreadFunction, this, m_Poriority, m_StackSize)== 0 ? TRUE : FALSE;

    return bRet;
}

void CThreadA::Exit(void)
{
    DBG_POOL("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    if (THREAD_STOPED == m_ThreadState)    //判断线程是否要退出
    {
        DBG_POOL("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        ThreadExit();
    }
}

BOOL CThreadA::Wakeup(void)
{
    m_ThreadSemaphore.Post();
    return TRUE;
}

void CThreadA::SetThreadName(char *thrname)
{
    m_ThreadName = string(thrname);
}


void CThreadA::SetPriority(int priority)
{
    m_Poriority = priority;
}

int CThreadA::GetPriority(void)
{
    return m_Poriority;
}

int CThreadA::GetConcurrency(void)
{
    return 0;
}

void CThreadA::SetConcurrency(int num)
{


}

void CThreadA::Detach(void)
{
    m_Detach = TRUE;
}

void CThreadA::Join(void)
{
    DBG_POOL("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    if (!m_Detach)
    {
        DBG_POOL("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~id:%d\n", m_ThreadID);
        ThreadDestroy(m_hThread);
    }
}

BOOL CThreadA::Yield(void)
{
    return TRUE;
}

int CThreadA::Self(void)
{
    return 0;
}
