#ifndef CTHREAD_H_
#define CTHREAD_H_

#include <semaphore.h>
#include "CSemaphore.h"
#include "thread.h"

#define PORIORITY_DEF  64
#define STACK_SIZE_DEF 256 * 1024

class CThreadA
{
    public:
	enum ThreadState
	{
	    THREAD_SUSPENDED,
	    THREAD_RUNNING,
	    THREAD_STOPED,
	};
    private:
        int          m_ErrCode;
        CSemaphore    m_ThreadSemaphore;  //the inner semaphore, which is used to realize
        int            m_ThreadID;
        BOOL         m_Detach;       //The thread is detached
        BOOL         m_CreateSuspended;  //if suspend after creating
        string       m_ThreadName;
        ThreadState m_ThreadState;      //the state of the thread

	THREAD_HANDLE m_hThread;
	int m_Poriority;
	uint32_t m_StackSize;
	static int m_CreateCount;
        
    protected:
        void     SetErrcode(int errcode) {m_ErrCode = errcode;}
        static void ThreadFunction(void *pArg);
        
    public:
        CThreadA();
        CThreadA(BOOL createsuspended, BOOL detach);
        void ValInit(BOOL createsuspended, BOOL detach);
        virtual ~CThreadA();
        
        virtual void Run(void) = 0;
        void     SetThreadState(ThreadState state) {m_ThreadState = state;}
        BOOL     Terminate(void);    //Terminate the threa
        BOOL     Start(void);        //Start to execute the thread
        void     Exit(void);
        BOOL     Wakeup(void);
        ThreadState  GetThreadState(void) {return m_ThreadState;}
        int      GetLastError(void) {return m_ErrCode;}
        void     SetThreadName(char *thrname);
        char    *GetThreadName(void) {return (char*)m_ThreadName.c_str();}
        int      GetThreadID(void) {return m_ThreadID;}
        void     SetPriority(int priority);
        int      GetPriority(void);
        int      GetConcurrency(void);
        void     SetConcurrency(int num);
        void     Detach(void);
        void     Join(void);
        BOOL     Yield(void);
        int      Self(void);
};

#endif
