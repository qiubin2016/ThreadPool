#ifndef CTHREADMANAGE_H_
#define CTHREADMANAGE_H_

#include "CWorkerThread.h"


class CThreadManage
{
    private:
        CThreadPool    *m_Pool;
        int          m_NumOfThread;
        
    protected:
    
    public:
        CThreadManage();
        CThreadManage(int num);
        virtual ~CThreadManage();
        
        void     SetParallelNum(int num);
        void    Run(CJob *job, void *jobdata);
        void    TerminateAll(void);
};

#endif