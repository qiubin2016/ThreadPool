#ifndef __MISC_H__
#define __MISC_H__

#include "comdef.h"
#include <time.h>

#define HOST_BYTE_ORDER     false
#define NETWORK_BYTE_ORDER  true
#define 	LONE_CHAR(s, c)   ((s)[0] == (c) && !(s)[1])



typedef enum
{
  L_EQU_R = 0,
  L_LESS_R,
  L_GREATER_R
}CmpResult_en;




#ifdef __cplusplus
extern "C"{
#endif

uint32_t SystemGetMSCount(void);
void SystemSleep(uint32_t dwMilliSeconds);
void SystemSleepSec(uint32_t dwSeconds);
void Exit(int status);
int  System(const char *command);
void WaitMs(uint32_t dwMilliSeconds);
void WaitUs(uint32_t MicroiSeconds);

int SystemCmd(const char *str);
void GetSysTimeUs(char * pBuf);
void PrintSysTimeUs();    //imx6平台测试每次耗时几十us
void PrintSysTimeNs();    //imx6平台测试每次耗时10-20ms,不建议使用

void GetLocalTimeToStr(char *pTime);
int  GetLocalTimeToHex(uchar *pTime);
time_t  ConvertStrtoTime(const char * pTime);
int SetTime(const char* pTime);
int CmpCurrentTime(const unsigned char* pTime);
int GetWeek();
signed long long TimeSpan(struct timeval tv, struct timeval tv2);
signed long long TimeSpanMs(struct timeval tv_begin, struct timeval tv_end);

void  ConvertStrtoBCD(const char *pSrc, char * pTime);    //将字符串数组转换为年月日时分秒数组

DWORD Str2Ip(const char* pStr);
DWORD Byte2Ip(const char* pBuf);
int   Byte2Int(const char* pBuf, bool bFlag);


BYTE Xor(const BYTE *pBuf, uint32_t uilen);

BYTE HexToBcd(BYTE hex);
BYTE BcdToHex(BYTE bcd);
int  AscToHex(const uchar *in, uchar *out, size_t count);
int  StrToHex(const uchar *in, uchar *out);
int  StrToHexBigEndian(const uchar *in, uchar *out);
uchar* HexToStr(const uchar *in, uchar *out,  uint32 size);
uchar* HexToStrWithSpace(const uchar *in, uchar *out, uint32 size); //out的空间 = size*3+1
void HexToStrBigEndian(const uchar *in, uchar *out,  uint32 size);
//int StrToInt(const char *str, BOOL bIntegrityCheck);
int StrToInt(const char *str, int *value_result);


int Mount(const char *source, const char *target, const char *filesystemtype);
int Umount(const char *target);

int CreateDir(const char *pPathName) ;    //支持递归创建路径
int bb_make_directory(char *path, long mode);

int  init_setproctitle(char **os_argv);
void setproctitle(const char *title, char **os_argv);
void SetTitle(int argc, char **argv);

char *strptime(const char *s, const char *format, struct tm *tm);
void StrCpy(uint8_t *dst, const uint8_t *src, uint32_t len);
void StrSet(uint8_t *str, uint8_t dat, uint32_t len);
uint32_t StrCmp(const uint8_t *str_l, const uint8_t *str_r, uint32_t len);
void StrCat(uint8_t *str, uint8_t *str_l, uint8_t *str_r);
int CmpConstByte(const char *pSrc, char cConst, unsigned int uiLen);
int IsIpValid(const char *ip);    //检测IP是否合法;"192.168.123.134"
BOOL  StrCmpDir(const char *pDirName, char *pFileName);
long GetFileSize(const char * pFileName);
BOOL IsFileExist(const char *pFileName);

//BOOL IsNumValid(unsigned int iNum, const unsigned char* pBuf);
//int GetGotoFloor(const unsigned char *const pFloor, unsigned int uiMaxByte);
//int SetFloorNbr(unsigned int uiFloorNbr, unsigned char *const pFloor);
unsigned char CalculateCheck(const unsigned char*const pBuf, unsigned int uiLen);
int GetMACAddr(const char *const pEth, char * pBuf);
uint32_t BitCount(uint32_t v);
uint32_t GetBitCount(uint32_t v);
int ExtractPAN(const uchar *pCompressedCardNum, uint32_t uiSize,
               uchar *pCardNumber, uint32_t *pLen);
int Luhn(const uchar *pCardNumber, uint32_t size);
void ArrayBitSet(uchar *buf, uint32_t uiPos);
void ArrayBitClear(uchar *buf, uint32_t uiPos);
BOOL ArrayBitTest(uchar *buf, uint32_t uiPos);

#ifdef __cplusplus
}
#endif

#endif
