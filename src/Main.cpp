#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "Logging.h"
#include "misc.h"
#include "CJob.h"
#include "CThreadManage.h"
#include "Cpp.h"

using namespace std;

int app_dbg_level  = 0;
int vpu_dbg_level  = 0;
int card_dbg_level = 0;

class CXJob: public CJob
{
    public:
        CXJob() {/*int i = 0;*/}
        ~CXJob() {}
        void Run(void *jobdata)
        {
            int tmp_jobdata = *(int*)jobdata;
            delete (int*)jobdata;
            DBG("The Job comes from CXJob!No:%d begin,time:%s\n", tmp_jobdata, GetSysTimeUs().c_str());
            sleep(1);
            DBG("The Job comes from CXJob!No:%d end,time:%s\n", tmp_jobdata, GetSysTimeUs().c_str());
        }
};

class CYJob: public CJob
{
    public:
        CYJob() {/*int i = 0;*/}
        ~CYJob() {}
        void Run(void *jobdata)
        {
            DBG("The Job comes from CYJob\n");
        }
};

int main(int argc, char** argv)
{
	GET_APP_DEBUG_LEVEL();
	app_set_log_level_mask(app_dbg_level);
	DEBUG("app_dbg_level = %d\n", app_dbg_level);
	DEBUG_COLOR("color app_dbg_level = %d\n", app_dbg_level);
	DBG_BKG("app_dbg_level = %d\n", app_dbg_level);
    DBG("pid:%d\n", getpid());
    DBG_POOL("----------------------------------------------version:1.2----\n");
	// sleep(5);
    CThreadManage *manage = new CThreadManage(3);
    for (int i = 0; i < 4; i++)
    {
        CXJob   *job = new CXJob();
        //manage->Run(job, NULL);
        int *p = new int;
        *p = i + 1;
        manage->Run(job, (void*)p);
    }
    DBG("\n");
    sleep(20);
//    CYJob *job = new CYJob();
//    manage->Run(job, NULL);
    DBG("\n");

    // for (int i = 0; i < 40; i++)
    // {
    //     CXJob   *job = new CXJob();
    //     //manage->Run(job, NULL);
    //     int *p = new int;
    //     *p = i + 1;
    //     manage->Run(job, (void*)p);
    // }

    sleep(1);
    manage->TerminateAll();
    DBG("\n");
    // while(1)
    //     sleep(10);
	
	return 0;
}
