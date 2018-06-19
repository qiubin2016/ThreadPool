#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "Logging.h"
#include "misc.h"
#include "CJob.h"
#include "CThreadManage.h"

using namespace std;

int app_dbg_level  = 0;
int vpu_dbg_level  = 0;
int card_dbg_level = 0;

class CXJob: public CJob

{

    public:
    
        CXJob() {int i = 0;}
        
        ~CXJob() {}
        
        void Run(void *jobdata)
        {
            DBG("The Job comes from CXJob!No:%d\n", *(int*)jobdata);
            delete (int*)jobdata;
            sleep(1);
        }
        
};



class CYJob: public CJob

{

    public:
    
        CYJob() {int i = 0;}
        
        ~CYJob() {}
        
        void Run(void *jobdata)
        {
            DBG("The Job comes from CYJob\n");
        }
        
};



main()
{
	GET_APP_DEBUG_LEVEL();
	app_set_log_level_mask(app_dbg_level);
	DEBUG("app_dbg_level = %d\n", app_dbg_level);
	DEBUG_COLOR("color app_dbg_level = %d\n", app_dbg_level);
	DBG_BKG("app_dbg_level = %d\n", app_dbg_level);
    DBG("pid:%d\n", getpid());
    DBG_POOL("----------------------------------------------\n");
	// sleep(5);
    CThreadManage *manage = new CThreadManage(3);
    for (int i = 0; i < 15; i++)
    {
        CXJob   *job = new CXJob();
        //manage->Run(job, NULL);
        int *p = new int;
        *p = i + 1;
        manage->Run(job, (void*)p);
    }
    DBG("\n");
    sleep(5);
    CYJob *job = new CYJob();
    manage->Run(job, NULL);
    DBG("\n");

    // for (int i = 0; i < 40; i++)
    // {
    //     CXJob   *job = new CXJob();
    //     //manage->Run(job, NULL);
    //     int *p = new int;
    //     *p = i + 1;
    //     manage->Run(job, (void*)p);
    // }

    // sleep(5);
    manage->TerminateAll();
    DBG("\n");
    // while(1)
    //     sleep(10);
}
