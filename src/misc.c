#include "misc.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <ctype.h>


const char hex_asc[] = "0123456789abcdef";
#define hex_asc_lo(x)   hex_asc[((x) & 0x0f)]
#define hex_asc_hi(x)   hex_asc[((x) & 0xf0) >> 4]
/**
 * hex_to_bin - convert a hex digit to its real value
 * @ch: ascii character represents hex digit
 *
 * hex_to_bin() converts one hex digit to its actual value or -1 in case of bad
 * input.
 */
int hex_to_bin(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - '0';
    ch = tolower(ch);
    if ((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    return -1;
}
/**
 * hex2bin - convert an ascii hexadecimal string to its binary representation
 * @dst: binary result
 * @src: ascii hexadecimal string
 * @count: result length
 *
 * Return 0 on success, -1 in case of bad input.
 */
int hex2bin(unsigned char *dst, const char *src, size_t count)
{
    while (count--) {
        int hi = hex_to_bin(*src++);
        int lo = hex_to_bin(*src++);

        if ((hi < 0) || (lo < 0))
            return -1;

        *dst++ = (hi << 4) | lo;
    }
    return 0;
}

static void TimeCharToStr(int iYear, int iMon, int iDay, int iHour, int iMin, int iSec, char *pStr);

uint32_t SystemGetMSCount(void)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (unsigned int)(now.tv_sec*1000+(double)now.tv_nsec/1000000);
}

void SystemSleep(uint32_t dwMilliSeconds)
{
	usleep(dwMilliSeconds* 1000);
}

void SystemSleepSec(uint32_t dwSeconds)
{
	sleep(dwSeconds);
}

void Exit(int status)
{
	exit(status);
}

int System(const char *command)
{
	return system(command);
}

void WaitMs(uint32_t ms)
{
	usleep(ms * 1000);
}

void WaitUs(uint32_t us)
{
	usleep(us);
}


int SystemCmd(const char *str)
{
#ifndef __DEBUG__
	char cmd[200] = {0};

	sprintf(cmd, "%s>/dev/null", str);
	return system(cmd);
#else
	return system(str);
#endif

}

void GetSysTimeUs(char * pBuf)
{
    struct timeval    tv;
    struct tm         *pTm;
    int iRet;

    gettimeofday(&tv, NULL);
    pTm = localtime(&tv.tv_sec);
    iRet = sprintf(pBuf, "%04d-%02d-%02d %02d:%02d:%02d.%06ld",
    		1900+pTm->tm_year, 1 + pTm->tm_mon, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec,  tv.tv_usec);
    //If an output error is encountered, a negative value is returned
    //A null character is written at the 	end of the characters written
    if (iRet < 0)
    {
    	pBuf[0] = '\0';
    }
}

void PrintSysTimeUs()    //imx6平台测试每次耗时几十us
{
    char cTime[100];

    GetSysTimeUs(cTime);
    DEBUG("%s", cTime);
}

void PrintSysTimeNs()
{
	system("date +%F_%H:%M:%S.%N");    //imx6平台测试每次耗时10-20ms,不建议使用
}

static void TimeCharToStr(int iYear, int iMon, int iDay, int iHour, int iMin, int iSec, char *pStr)
{
	sprintf(pStr, "%04d-%02d-%02d %02d:%02d:%02d", iYear, iMon, iDay, iHour, iMin, iSec);
}

//当前时间'2017-10-09 15:19:30 星期一'转换成0x17 0x10 0x09 0x15 0x19 0x30 0x01
int GetLocalTimeToHex(uchar *pTime)
{
	time_t     Time;
	struct tm  *pTm;

	Time = time(NULL);
	if(-1 == Time)
	{
		return -1;
	}
//	pTm = gmtime(&Time);    //格林尼治时区的时间
	pTm = localtime(&Time);    //当前时区的时间
	if(pTm == NULL)
	{
		return -1;
	}
	int year = 1900 + pTm->tm_year - 2000;
	if((year < 0) || (year >= 100)) //2000-2099合法
	{
		year = 0;
	}
	pTime[0] = HexToBcd(year);	// year
	pTime[1] = HexToBcd(1 + pTm->tm_mon); //month   pTm->tm_mon 的范围是[0-11]
	pTime[2] = HexToBcd(pTm->tm_mday);    // day [1-31]
	pTime[3] = HexToBcd(pTm->tm_hour);    // hour [0-23]
	pTime[4] = HexToBcd(pTm->tm_min);     // min [0-59]
	pTime[5] = HexToBcd(pTm->tm_sec);     // sec [0-60] 60只有润秒时存在
	pTime[6] = HexToBcd(pTm->tm_wday ? pTm->tm_wday : 7);    // week pTm->tm_wday的范围是[0-6]，0代表星期天

	return 0;
}

//将时间转换为字符串:pTime[...]:"2017-01-04 10:29:30"
void GetLocalTimeToStr(char *pTime)
{
	time_t Time;
	struct tm * pTm;

	time(&Time);
	pTm = localtime(&Time);    //当前时区的时间

	TimeCharToStr(1900 + pTm->tm_year, 1 + pTm->tm_mon, pTm->tm_mday, pTm->tm_hour, pTm->tm_min,  pTm->tm_sec, pTime);
}

//pTime:"2017-02-28 17:28:30"
int SetLocalTime(const char * pTime)
{
	struct timeval TimeVal;

	TimeVal.tv_sec = ConvertStrtoTime(pTime);
	TimeVal.tv_usec = 50000;    //50ms

	return (settimeofday(&TimeVal, NULL));
}

//pTime:year(2B) + Mon(1B) + Day(1B) + Hour(1B) + Min(1B) + Sec(1B)
int SetTime(const char* pTime)
{
	int iRet = -1;
	char pStr[64];
	int iYear = (pTime[0] >> 4) * 1000 + (pTime[0] & 0x0f) * 100 +  (pTime[1] >> 4) * 10 + (pTime[1] & 0x0f) ;
	int iMon = (pTime[2] >> 4) * 10 + (pTime[2] & 0x0f);
	int iDay = (pTime[3] >> 4) * 10 + (pTime[3] & 0x0f);
	int iHour = (pTime[4] >> 4) * 10 + (pTime[4] & 0x0f);
	int iMin = (pTime[5] >> 4) * 10 + (pTime[5] & 0x0f);
	int iSec = (pTime[6] >> 4) * 10 + (pTime[6] & 0x0f);
	//转换成"2017-02-28 17:28:30"
	TimeCharToStr(iYear, iMon, iDay, iHour, iMin, iSec, pStr);

	//将时间设置到系统时间
	iRet = SetLocalTime(pStr);
	if (0 == iRet)
	{
		//将系统时间设置到RTC
		iRet = system("hwclock -w");
	}

	return iRet;
}

/*
函数功能：输入的时间跟当前系统时间进行对比
输入参数：pTime,6个字节，例如，有效期2017-09-28 14:00:30，则pTime[0-5]:{0x17, 0x09, 0x28, 0x14, 0x30};
输出参数：无
函数返回值：-1-输入参数非法，0-输入的时间小于当前时间，1-时间相等，2-输入的时间大于当前时间；
*/
int CmpCurrentTime(const unsigned char* pTime)
{
	int iRet = -1;
	int i;
	unsigned char ucTime[7];  //年月日时分秒星期

	if (NULL != pTime)
	{
		GetLocalTimeToHex(ucTime);  //当前时间'2017-10-09 15:19:30 星期一'转换成0x17 0x10 0x09 0x15 0x19 0x30 0x01
		for (i = 0; i < 5; i++)
		{
//			DEBUG("period:%02x, curr:%02x\n", pTime[i], ucTime[i]);
			if (pTime[i] != ucTime[i])
			{
				iRet = 0;
				break;
			}
		}
		if (5 == i)
		{
			i--;
		}
		if (pTime[i] > ucTime[i])
		{
			iRet = 2;
		}
		else if (pTime[i] == ucTime[i])
		{
			iRet = 1;
		}
		else
		{
			iRet = 0;
		}
	}

	return iRet;
}

//获取当前星期几
int GetWeek()
{
	struct timeval    tv;
	struct tm         *pTm;

	gettimeofday(&tv, NULL);
	pTm = localtime(&tv.tv_sec);
	return (pTm->tm_wday ? pTm->tm_wday : 7);
}

//a time span up to microsecond resolution.
signed long long TimeSpan(struct timeval tv_begin, struct timeval tv_end)
{
	return (signed long long)(tv_end.tv_sec - tv_begin.tv_sec)*1000000  \
			               + (tv_end.tv_usec - tv_begin.tv_usec);
}

//a time span up to microsecond resolution.
signed long long TimeSpanMs(struct timeval tv_begin, struct timeval tv_end)
{
	return (signed long long)(tv_end.tv_sec - tv_begin.tv_sec)*1000  \
				           + (tv_end.tv_usec - tv_begin.tv_usec)/1000;
}

void  ConvertStrtoBCD(const char *pSrc, char * pTime)
{
	//将"2017-02-27 01:23:30"转换为pTime[0-7]:0x20 0x17 0x02 0x27 0x01 0x23 0x30
    struct tm Tm = {0};
    int i = 0;
    unsigned char ucTmp;

    strptime(pSrc, "%Y-%m-%d %H:%M:%S", &Tm);   //将字符串转换为tm时间
    Tm.tm_isdst = -1;
//    DEBUG("y=%d,m=%d,d=%d,h=%d,m=%d,s=%d\n", Tm.tm_year, Tm.tm_mon, Tm.tm_mday, Tm.tm_hour, Tm.tm_min, Tm.tm_sec);
    ucTmp = (unsigned char)((Tm.tm_year + 1900) / 100);    //年
    pTime[i++] = ((ucTmp / 10) << 4) + ucTmp % 10;    //20-->0x16-->0x20
    ucTmp = (Tm.tm_year + 1900) % 100;
    pTime[i++] = ((ucTmp / 10) << 4) + ucTmp % 10;    //17-->0x11-->0x17
    ucTmp = (unsigned char)(Tm.tm_mon + 1);
	pTime[i++] = ((ucTmp / 10) << 4) + ucTmp % 10;    //月
	ucTmp = (unsigned char)(Tm.tm_mday);
	pTime[i++] = ((ucTmp / 10) << 4) + ucTmp % 10;    //日
	ucTmp = (unsigned char)(Tm.tm_hour);
	pTime[i++] = ((ucTmp / 10) << 4) + ucTmp % 10;    //时
	ucTmp = (unsigned char)(Tm.tm_min);
	pTime[i++] = ((ucTmp / 10) << 4) + ucTmp % 10;    //分
	ucTmp = (unsigned char)(Tm.tm_sec);
	pTime[i++] = ((ucTmp / 10) << 4) + ucTmp % 10;    //秒
}

DWORD Str2Ip(const char* pStr)
{
	signed char j=0;
	unsigned char cIp;
	DWORD iRet = 0;
	const char* p = pStr;

	cIp = (unsigned char)atoi(p);
	iRet |= (((DWORD)(cIp))<<(j*8));
	j++;

	while((*p != 0) &&(j>=0))
	{
		if(*p != '.')
		{
			p++;
			continue;
		}

		cIp = (unsigned char)atoi(++p);
		iRet |= (((DWORD)(cIp))<<(j*8));
		j++;
	}

	return iRet;
}

DWORD Byte2Ip(const char* pBuf)
{
	return ((pBuf[0]<< 24) + (pBuf[1]<< 16) + (pBuf[2]<< 8) + (pBuf[3]));
}

int Byte2Int(const char* pBuf, bool bFlag)
{
	int iRet = (pBuf[0]<< 24) + (pBuf[1]<< 16) + (pBuf[2]<< 8) + (pBuf[3]);

	if(bFlag == NETWORK_BYTE_ORDER)
	{
		iRet = htonl(iRet);
	}

	return  iRet;
}

BYTE Xor(const BYTE *pBuf, uint32_t uiLen)
{
	BYTE ucResult = 0x00;

	while(uiLen--)
	{
		ucResult ^= *(pBuf++);
	}

	return ucResult;
}

// 将字符串转换为整数，若传入的参数非法，则返回0，同时置errno值
// “2”----返回2
// “+2”---返回2
// “-2”---返回-2
// “S2”---返回0，置errno为ERANGE
// “2S”---如果完整性检查标志bIntegrityCheck为FALSE 返回2，
//        如果完整性检查标志bIntegrityCheck为TRUE，返回0，置errno为ERANGE
#if(1 == 0)
int StrToInt(const char *str, BOOL bIntegrityCheck)
{
    int base = 10;
    char *endptr;
    long val;


    errno = 0;    /* To distinguish success/failure after call */
    val = strtol(str, &endptr, base);

    /* Check for various possible errors */

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0)) {
        DEBUG("strtol error:%s\n", strerror(errno));
        val = 0;
        //errno = ERANGE;
    }

    if (endptr == str) {
        DEBUG("No digits were found\n");
        val = 0;
        errno = ERANGE;
    }

    /* If we got here, strtol() successfully parsed a number */

    if (*endptr != '\0')        /* Not necessarily an error... */
    {
        DEBUG("Further characters after number: %s\n", endptr);
        if(bIntegrityCheck == TRUE)
        {
            val = 0;
            errno = ERANGE;
        }
    }

    return val;
}
#else
int StrToInt(const char *str, int *value_result)
{
    int base = 10;
    char *endptr;
    long val;
    int result = 0;

    errno = 0;    /* To distinguish success/failure after call */
    val = strtol(str, &endptr, base);

    /* Check for various possible errors */

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0)) {
        DEBUG("strtol error:%s\n", strerror(errno));
        result = -1;
        val = 0;
        goto _end;
    }

    if (endptr == str) {
        DEBUG("No digits were found\n");
        result = -1;
        val = 0;
        goto _end;
    }

    /* If we got here, strtol() successfully parsed a number */

    if (*endptr != '\0')        /* Not necessarily an error... */
    {
        DEBUG("Further characters after number: %s\n", endptr);
        if(value_result && *value_result == TRUE)
        {
            result = endptr - str;
            val = 0;
            goto _end;
        }
    }
_end:
    if(value_result)
    {
        *value_result = result;
    }
    return val;
}
#endif

BYTE HexToBcd(BYTE hex)
{
	return (((hex/10)<<4) +(hex%10));
}

BYTE BcdToHex(BYTE bcd)
{
	return ((bcd>>4)*10 +(bcd & 0x0F));
}

// "1234"->0x12 0x34
int AscToHex(const uchar *in, uchar *out, size_t count)
{
    int iRet;

    if((NULL == in) || (NULL == out))
    {
        return -1;
    }

    iRet = hex2bin(out, (const char*)in, count/2);
    if(iRet == 0)
    {
        if(count%2) //"123"->0x12 0x30
        {
            uchar buf[2] = { in[count-1], '0'};
            buf[0] = in[count-1];
            buf[1] = '0';
            iRet = hex2bin(&out[count/2], (const char*)buf, 1);
        }
    }

    return iRet;

}
int StrToHex(const uchar *in, uchar *out)
{
    return AscToHex(in, out, strlen(in));
}

// "1234"->0x34 0x12
int StrToHexBigEndian(const uchar *in, uchar *out)
{
	int high;
	int low;
	int outsize;

	if ((NULL == in) || (NULL == out))
	{
		return -1;
	}

	outsize = (strlen((const char*)in) + 1) / 2;
	*(out + outsize) = '\0';
    while(*in)
    {
        high = hex_to_bin(*in++);
		if(*in)    // "1234"->0x34 0x12
		{
			low = hex_to_bin(*in++);
		}
		else     //"123"->0x30 0x12
		{
			low = 0;
		}
        if ((high < 0) || (low < 0))
        {
            return -1;
        }
        *(out + outsize - 1) = (uchar)((high<<4)|low);
        outsize--;
    }

    return 0;
}

uchar* HexToStr(const uchar *in, uchar *out,  uint32 size)//out的空间 = size*2+1
{
    int high;
    int low;
    uchar *p = out;

	if ((NULL == in) || (NULL == out))
	{
		return NULL;
	}

    while(size)
    {
        high = (*in)>>4;
        low  = (*in)&0x0F;
        *(out++) = (high > 9) ? high-10 + 'A' : high + '0';
        *(out++) = (low > 9) ? low - 10 + 'A' : low + '0';
        in++;
        size--;
    }
    *out='\0';

    return p;
}

uchar* HexToStrWithSpace(const uchar *in, uchar *out, uint32 size)//out的空间 = size*3+1
{

	// pointer to the first item (0 index) of the output array
	char *ptr = (char *)&out[0];

	int i;
	for (i = 0; i < size; i++)
	{
		/* sprintf converts each byte to 2 chars hex string and a null byte, for example
		 * 10 => "0A\0".
		 *
		 * These three chars would be added to the output array starting from
		 * the ptr location, for example if ptr is pointing at 0 index then the hex chars
		 * "0A\0" would be written as output[0] = '0', output[1] = 'A' and output[2] = '\0'.
		 *
		 * sprintf returns the number of chars written execluding the null byte, in our case
		 * this would be 2. Then we move the ptr location two steps ahead so that the next
		 * hex char would be written just after this one and overriding this one's null byte.
		 *
		 * We don't need to add a terminating null byte because it's already added from
		 * the last hex string. */
		ptr += sprintf (ptr, "%02X ", in[i]);
	}
	return (uchar*)&out[0];
}



void HexToStrBigEndian(const uchar *in, uchar *out,  uint32 size)//out的空间 = size*2+1
{
    int high;
    int low;
    unsigned char value;

	if ((NULL == in) || (NULL == out))
	{
		return;
	}

    while(size)
    {
    	value = *(in + size - 1);
        high = value >> 4;
        low  = value & 0x0F;
        *(out++) = (high > 9) ? high - 10 + 'A' : high + '0';
        *(out++) = (low > 9) ? low - 10 + 'A' : low + '0';
        size--;
    }
    *out = '\0';
}

int Mount(const char *source, const char *target, const char *filesystemtype)
{
	return mount(source, target, filesystemtype, 0, NULL);
}

int Umount(const char *target)
{
	return umount(target);
}

time_t  ConvertStrtoTime(const char * pTime)
{
    struct tm Tm = {0};
    time_t Time = 0;

    strptime(pTime, "%Y-%m-%d %H:%M:%S", &Tm);   //将字符串转换为tm时间
    Tm.tm_isdst = -1;
//    DEBUG("%s:%d,ptime=%s, y=%d,m=%d,d=%d,h=%d,m=%d,s=%d\n", __func__, __LINE__, pTime, Tm.tm_year, Tm.tm_mon, Tm.tm_mday, Tm.tm_hour, Tm.tm_min, Tm.tm_sec);
    Time  = mktime(&Tm);                         //将tm时间转换为秒时间
    //t_ += 3600;                                //秒数加3600
    return Time;

}

int CreateDir(const char *pPathName)
{
	char DirName[512];
	int i, len;


	//创建路径,先检测该目录是否已存在
	DIR *pDir = opendir(pPathName);
	if (NULL == pDir)
	{
		//创建目录
		 strcpy(DirName, pPathName);
		len = strlen(DirName);
		if(DirName[len-1] != '/')
		{
			strcat(DirName, "/");
		}

		len = strlen(DirName);

		for (i = 1; i < len; i++)
		{
			if (DirName[i] == '/')
			{
				DirName[i] = 0;
				if (access(DirName, F_OK) != 0)
				{
					if (mkdir(DirName, 0755) == -1)
					{
						perror("mkdir error");
						return -1;
					}
				}
				DirName[i] = '/';
			}
		}
	}
	else
	{
		closedir(pDir);
	}

    return 0;
}



int  bb_make_directory(char *path, long mode)
{
	int flags;
	mode_t cur_mask;
	mode_t org_mask;
	const char *fail_msg;
	char *s;
	char c;
	struct stat st;

	/* "path" can be a result of dirname().
	 * dirname("no_slashes") returns ".", possibly read-only.
	 * musl dirname() can return read-only "/" too.
	 * We need writable string. And for "/", "." (and ".."?)
	 * nothing needs to be created anyway.
	 */
	if (LONE_CHAR(path, '/'))  //path = "/"
	{
		return 0;
	}
	if (path[0] == '.')
	{
		if (path[1] == '\0') 	//path = "."
		{
			return 0;
		}
//		if (path[1] == '.' && path[2] == '\0') //path = ".."
//		{
//			return 0;
//		}
	}

	org_mask = cur_mask = (mode_t)-1L;
	s = path;
	while (1)
	{
		c = '\0';


		// Get the parent
		// Bypass leading non-'/'s and then subsequent '/'s
		while (*s)
		{
			if (*s == '/')
			{
				do
				{
					++s;
				} while (*s == '/');
				c = *s; // Save the current char
				*s = '\0'; // and replace it with nul
				break;
			}
			++s;
		}

		if (c != '\0')
		{
			/* Intermediate dirs: must have wx for user */
			if (cur_mask == (mode_t)-1L) // wasn't done yet?
			{
				mode_t new_mask;
				org_mask = umask(0);
				cur_mask = 0;
				/* Clear u=wx in umask - this ensures
				 * they won't be cleared on mkdir */
				new_mask = (org_mask & ~(mode_t)0300);
				if (new_mask != cur_mask)
				{
					cur_mask = new_mask;
					umask(new_mask);
				}
			}
		}
		else
		{
			/* Last component: uses original umask */
			if (org_mask != cur_mask)
			{
				cur_mask = org_mask;
				umask(org_mask);
			}
		}

		if (mkdir(path, 0777) < 0)
		{
			/* If we failed for any other reason than the directory
			 * already exists, output a diagnostic and return -1 */

			if ((errno != EEXIST && errno != EISDIR)
			 || ((stat(path, &st) < 0) || !S_ISDIR(st.st_mode)))
			{
				fail_msg = "create";
				break;
			}
			/* Since the directory exists, don't attempt to change
			 * permissions if it was the full target.  Note that
			 * this is not an error condition. */
			if (!c)
			{
				goto ret0;
			}
		}

		if (!c)
		{
			/* Done.  If necessary, update perms on the newly
			 * created directory.  Failure to update here _is_
			 * an error. */
			if ((mode != -1) && (chmod(path, mode) < 0))
			{
				fail_msg = "set permissions of";
				break;
			}
			goto ret0;
		}

		/* Remove any inserted nul from the path (recursive mode) */
		*s = c;
	} /* while (1) */

	flags = -1;
	DEBUG("can't %s directory '%s'", fail_msg, path);
	goto ret;
 ret0:
	flags = 0;
 ret:
	if (org_mask != cur_mask)
	{
		umask(org_mask);
	}
	return flags;
}


/*
********************************************************************************
  函数名称: StrCpy
  函数功能: 字符串拷贝
  输入参数: *dst--目标字符串
           *src--源字符串
           len---要复制的字符串长度
  输出参数:
  函数返回:
  使用资源:
  注意事项:
  算法思路:
*******************************************************************************
*/
void StrCpy(uint8_t *dst, const uint8_t *src, uint32 len)
{
	while(len)
	{
		*dst++ = *src++;
		len--;
	}
}

/*
********************************************************************************
  函数名称: StrSet
  函数功能: 字符串设置
  输入参数: *dst--目标字符串
            dat---源字符串
            len---要复制的字符串长度
  输出参数:
  函数返回:
  使用资源:
  注意事项:
  算法思路:
*******************************************************************************
*/
void StrSet(uint8_t *dst, uint8_t dat, uint32 len)
{
	while(len)
	{
		*dst++ = dat;
		len--;
	};
}

/*
********************************************************************************
  函数名称: StrCmp
  函数功能: 字符串比较
  输入参数: *str_l--数据串
            *str_r--数据串2
            len----要比较的数据串长度
  输出参数: 无
  函数返回: 0------str_l = str_r
            1------str_l < str_r
            2------str_l > str_r
  使用资源:
  注意事项: 低位地址为高数量级数据
  算法思路: 逐一比较,只要一个字节不同就马上退出,返回失败
            比较全部字节后都相同,返回成功.
********************************************************************************
*/
uint32 StrCmp(const uint8_t *str_l, const uint8_t *str_r, uint32 len)
{
  while(len)
  {
    if(*str_l != *str_r)
    {
      if(*str_l < *str_r)
      {
        return (L_LESS_R);
      }
      else
      {
        return (L_GREATER_R);
      }
    }
    str_l++;
    str_r++;
    len--;
  };

  return (L_EQU_R);
}

/*
********************************************************************************
  函数名称: StrCat
  函数功能: 字符串连接
  输入参数: *str----保存连接后的数据串
            *str_l--待连接的字符串1
            *str_r--待连接的字符串2
  输出参数: *str----连接后的字符串
  函数返回:
  使用资源:
  注意事项:
********************************************************************************
*/
void StrCat(uint8_t *str, uint8_t *str_l, uint8_t *str_r)
{
  while(*str_l)
  {
    *(str++) = *(str_l++);
  }
  *(str++) = '/';
  while(*str_r)
  {
    *(str++) = *(str_r++);
  }
  *str = '\0'; //zzh-20120703
}

/*
********************************************************************************
  函数名称: CmpConstByte
  函数功能: 比较数组的所有数据是否均等于某一个值
  输入参数: pSrc--要比较的数组
           cConst--字符串是否均等于该值
           uiLen--要比较的长度
  输出参数: 0--相等，>0--不相等
  函数返回:
  使用资源:
  注意事项:
********************************************************************************
*/
int CmpConstByte(const char *pSrc, char cConst, unsigned int uiLen)
{
	unsigned int i;

	for (i = 1; i < uiLen; i++) //比较前uiLen-1个字节 uiLen==0时怎么处理？
	{
		if (*pSrc != cConst)
		{
			break;
		}
		pSrc++;
	}

	return abs(*pSrc - cConst);
}

//0--IpValid;1--IpUnvalid;
int IsIpValid(const char *ip)
{
    unsigned int section = 0;  //每一节的十进制值
    int dot = 0;       //几个点分隔符

    while(*ip)
    {
        if(*ip == '.')
        {
            dot++;
            if(dot > 3)
            {
                return -1;
            }
            else if (1 == dot)    //IP第一节不允许为0
            {
            	if((0 == section) || (section > 254))
				{
					return -1;
				}
            	section = 0;
            }
            else
            {
            	if(section > 254)
				{
					return -1;
				}
				section = 0;
            }
        }
        else if(*ip >= '0' && *ip <= '9')
        {
            section = section * 10 + *ip - '0';
        }
        else
        {
            return -1;
        }
        ip++;
    }
    if((3 != dot) || (0 == section) || (section > 254))    //IP第四节不允许为0
	{
		return -1;
	}
    return 0;
}

//pDirName路径下查找pFileName文件是否存在.
BOOL  StrCmpDir(const char *pDirName, char *pFileName)
{
	struct dirent    *pDirp;
	DIR              *pDp;
	BOOL          bRet = false;

	if(NULL == (pDp = opendir(pDirName)))
	{
		DEBUG("open dir failed! dir: %s", pDirName);
		return -1;
	}
	while(NULL != (pDirp = readdir(pDp)))
	{
		if(0 == strcmp(pDirp->d_name, pFileName))
		{
			bRet = true;
			break;
		}
	}

	closedir(pDp);

	return bRet;
}

long GetFileSize(const char * pFileName)
{
	struct stat Stat;
	if (-1 == stat(pFileName, &Stat))
	{
		return -1;
	}
	return (long)Stat.st_size;
}

BOOL IsFileExist(const char *pFileName)
{
	BOOL bRet = false;

	if (0 == access(pFileName, F_OK))    // 判断文件是否存在
	{
		if (GetFileSize(pFileName) > 0)    //判断是否为空文件
		{
			bRet = true;
		}
	}
	return bRet;
}

/*
********************************************************************************
  函数名称：CalculateCheck
  函数功能：计算异或校验码
  输入参数：待计算校验的数据(含校验码)
  输出参数：返回值：异或校验值
  使用资源：
  注意事项：当将数据+校验码一起计算时，结果是0表示校验通过；
  算法思路：
********************************************************************************
*/
unsigned char CalculateCheck(const unsigned char*const pBuf, unsigned int uiLen)
{
    unsigned char ucCheckSum = 0;
    unsigned int i;

    if (NULL != pBuf)
    {
        for (i = 0; i < uiLen; i++)
        {
            ucCheckSum ^= pBuf[i];
        }
    }
    else
    {
        ucCheckSum = 0xFF;
    }

    return ucCheckSum;
}

//获取mac地址
int GetMACAddr(const char *const pEth, char * pBuf)
{
    int iRet = 0;
    char cFileName[80] = {0};
    FILE *pFp;
    char cBuf[20];
    int iBuf[6];

    if (NULL == pEth)
    {
        DEBUG("eth* err!\n");
        iRet = -1;
        return iRet;
    }
    sprintf(cFileName, "/sys/class/net/%s/address", pEth);
    pFp = fopen(cFileName, "rt");
    if (NULL == pFp)
    {
        DEBUG("open %s failed!\n", cFileName);
        iRet = -1;
        return iRet;
    }
    fread(cBuf, 1, sizeof(cBuf), pFp);
    fclose(pFp);

//    DEBUG("cbuf:%s\n", cBuf);
    //convert to integer
    sscanf(cBuf, "%x:%x:%x:%x:%x:%x", &iBuf[0], &iBuf[1], &iBuf[2], &iBuf[3], &iBuf[4], &iBuf[5]);
    pBuf[0] = (char)iBuf[0];
    pBuf[1] = (char)iBuf[1];
    pBuf[2] = (char)iBuf[2];
    pBuf[3] = (char)iBuf[3];
    pBuf[4] = (char)iBuf[4];
    pBuf[5] = (char)iBuf[5];
//    DEBUG("%02x %02x %02x %02x %02x %02x \n", pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf[4], pBuf[5]);

    return iRet;
}

// count the number of bits set in v
// https://www.cnblogs.com/graphics/archive/2010/06/21/1752421.html
// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
uint32_t BitCount(uint32_t v)
{
#if(1 == 0)
    uint32_t c; // c accumulates the total bits set in v
    for (c = 0; v; c++)
    {
      v &= v - 1; // clear the least significant bit set
    }
    return c;
#else
    uint32_t c; // c accumulates the total bits set in v

    v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
    c = (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24; // count

    return c;
#endif

}
//该算法效率更高
//http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
uint32_t GetBitCount(uint32_t v)
{
    uint32_t c; // c accumulates the total bits set in v

    v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
    c = (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24; // count

    return c;
}
//0x62 0x26 0x3F 0xFF->6 2 2 6 3
int ExtractPAN(const uchar *pCompressedCardNum, uint32_t uiSize,
                uchar *pCardNumber, uint32_t *pLen)
{
    uint32_t i;
    uint32_t j;
    uchar    ucTmp;
    for(i = 0, j = 0; i < uiSize; i++)
    {
        ucTmp = ((pCompressedCardNum[i]>>4) & 0x0F);
        if(ucTmp > 9)
        {
            break;
        }
        pCardNumber[j++] = ucTmp;
        ucTmp = (pCompressedCardNum[i] & 0x0F);
        if(ucTmp > 9)
        {
            break;
        }
        pCardNumber[j++] = ucTmp;
    }
    if(pLen != NULL)
    {
        *pLen = j;
    }

    return j;
}
// http://blog.csdn.net/songbohr/article/details/5261652
// http://blog.csdn.net/hiboy_111/article/details/47171451
// https://en.wikipedia.org/wiki/Luhn_algorithm#cite_note-0
int Luhn(const uchar *pNum, uint32_t size)
{
    static int table[2][10] = { {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                                {0, 2, 4, 6, 8, 1, 3, 5, 7, 9}
                              };
    const uchar *p = pNum;
    int odd = 1;
    int sum = 0;
    int i;
    uchar buf[size*2];
    size = ExtractPAN(pNum, size, buf, NULL);
    p = buf;
    for(i = size-1; i >= 0; i--)
    {
        if(p[i]>9)
        {
            return -1;
        }

        sum += table[(odd = 1 - odd)] [p[i]];
    }

    sum %= 10;
    return ( sum ? 10 - sum : 0 ) ; // return the check digit
}

//字符数组的uiPos(从左向右数)位置1
void ArrayBitSet(uchar *buf, uint32_t uiPos)
{
    buf[(uiPos-1)/8] |= (uchar)(1 << (8-((uiPos-1)%8)-1));
}
//字符数组的uiPos(从左向右数)位清0
void ArrayBitClear(uchar *buf, uint32_t uiPos)
{
    buf[(uiPos-1)/8] &= ~(uchar)(1 << (8-((uiPos-1)%8)-1));
}

BOOL ArrayBitTest(uchar *buf, uint32_t uiPos)
{
    return ((buf[(uiPos-1)/8] & (uchar)(1 << (8-((uiPos-1)%8)-1))) != 0);
}

