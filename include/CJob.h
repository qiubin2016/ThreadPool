#ifndef CJOB_H_
#define CJOB_H_

#include "CThreadA.h"

class CJob
{
public:
    CJob();
    virtual ~CJob();

    int      GetJobNo(void) const { return m_JobNo; }
    void     SetJobNo(int jobno) { m_JobNo = jobno;}
    char    *GetJobName(void) const { return m_JobName; }
    void     SetJobName(char *jobname);
    CThreadA *GetWorkThread(void) { return m_pWorkThread; }
    void     SetWorkThread ( CThreadA *pWorkThread )
    {
        m_pWorkThread = pWorkThread;
    }
    virtual void Run ( void *ptr ) = 0;
private:
    int      m_JobNo;        //The num was assigned to the job
    char    *m_JobName;      //The job name
    CThreadA  *m_pWorkThread;     //The thread associated with the job
};

#endif
