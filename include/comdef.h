#ifndef __COM_DEF_H__
#define __COM_DEF_H__

//#include "BuildConfig.h"

#ifndef  INCLUDE_H
	#include "Include.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef uint32_t
//#define uint32_t unsigned int
#endif

#ifndef FALSE
#define FALSE		0
#endif

#ifndef TRUE
#define TRUE		1
#endif

#ifndef uint64
#define uint64			unsigned long long
#endif

#ifndef BOOL
//#define BOOL 	unsigned char
#define BOOL 	bool
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif


#ifndef DWORD
#define DWORD unsigned int
#endif


#ifndef PARAM
#define PARAM	unsigned int
#endif

#define trace	printf
typedef union
{
	BYTE c[4];
	WORD s[2];
	DWORD l;
}IPADDR;


#define MACRO_JOIN(X,Y)	MACRO_DO_JOIN(X,Y)
#define MACRO_DO_JOIN(X,Y) 	MACRO_DO_JOIN2(X,Y)
#define MACRO_DO_JOIN2(X,Y)	X##Y

#define BITMSK(bit)		(int)(1<<(bit))


#define MAX_PATH		256

#ifndef CONFIG_DIR
	#define CONFIG_DIR		"./App"
#endif


typedef enum tagCFG_INDEX
{
	CFG_GENERAL	 = 0,
	CFG_NET,
	CFG_BACKGROUND,
	CFG_VOLUME,
	CFG_VOIP,
	CFG_YKT_SERVER,
	CFG_MJ,
	CFG_CARD,
    CFG_RCD,
    CFG_YKT_CFG,
    CFG_LINKAGE_SVR,
    CFG_MQTT,
    CFG_LOGINPARAM,
}CFG_INDEX;



#ifdef __cplusplus
}
#endif



#endif
