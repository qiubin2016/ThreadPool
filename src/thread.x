/*****************************************************
**
******************************************************/
//#include "Include.h"
#include "thread.h"

int ThreadCreate(pthread_t* pthread, void(*pStartAddress)(void* p), void* pParameter, int nProirity, uint32_t dwStackSize)
{
	pthread_attr_t attr;
	int ret;
	int retrytimes = 5;

	if(pthread == NULL || pStartAddress == NULL)
	{
		printf("ThreadCreate Failed!\n");
		return -1;
	}

	if(0 != (ret = pthread_attr_init(&attr)))
	{
		printf("%s:%d:pthread_attr_init failed!, ret:%d, errno:%s\n", __func__, __LINE__, ret, strerror(errno));
		return -1;
	}

	if(0 != (ret = pthread_attr_setstacksize(&attr, dwStackSize)))
	{
		printf("%s:%d:pthread_attr_setstacksize failed!, ret:%d, errno:%s\n", __func__, __LINE__, ret, strerror(errno));
		return -1;
	}

	//if (0 != (ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)))
	//{
	//	printf("%s:%d:pthread_attr_setdetachstate failed!, ret:%d, errno:%s\n", __func__, __LINE__, ret, strerror(errno));
	//	return -1;
	//}

	while(--retrytimes > 0)
	{
		if((ret = pthread_create((pthread_t*)pthread, &attr, (void*)pStartAddress, pParameter)) != 0)
		{
			usleep(20000);
			continue;
		}
		break;
	}

	if(retrytimes == 0 || pthread_attr_destroy(&attr) != 0)
	{
		printf("retrytimes = 0!\n");
		assert(0);
		return -1;
	}

	return 0;
}

int ThreadDestroy(THREAD_HANDLE hthread)
{
	int ret;
	ret = pthread_join(hthread, NULL);
	if(ret != 0)
	{
		return -1;
	}
	return 0;
}

void ThreadExit(void)
{
	pthread_exit(NULL);
}


int ThreadGetId(void)
{
	return syscall(__NR_gettid);
}


void ThreadSetName(const char* name)
{
	char title[16] = {0};
	strncpy(title, name, 15);
	title[15] = 0;

	prctl(PR_SET_NAME, title);
}
