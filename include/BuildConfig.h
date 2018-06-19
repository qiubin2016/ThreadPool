/*
 * BuildConfig.h
 *
 *  Created on: 2016年8月25日
 *      Author: zzh
 */

#ifndef INC_BUILDCONFIG_H_
#define INC_BUILDCONFIG_H_

// http://blog.csdn.net/benkaoya/article/details/52368638
// 用__attribute__((deprecated))管理过时的代码
#define attribute_deprecated __attribute__((deprecated))
#define attribute_unused     __attribute__ ((unused))

#define __DEBUG__
#define DBG_USE_FUNC

extern int app_dbg_level;
#ifdef __DEBUG__
#define GET_APP_DEBUG_LEVEL() 			\
	do   \
{	\
	char *dbg_env;	\
	dbg_env = getenv("APP_DBG");\
	app_dbg_level = dbg_env ? atoi(dbg_env) : 0;\
}while(0)

	//#define DEBUG(format, ...)  printf (format, ##__VA_ARGS__)
	//#define DEBUG(format, ...)  printf("FILE: "__FILE__", LINE: %d: "format"/n", __LINE__, ##__VA_ARGS__)
	//#define DEBUG(format, ...)  printf("FILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##__VA_ARGS__)
	//#define DEBUG(fmt, arg...)    printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg);
#ifndef DBG_USE_FUNC
	#define DEBUG(fmt, arg...) 			\
	do { 								\
		if (app_dbg_level > 1)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
		else if(app_dbg_level > 0)		\
			printf("[DBG]\t" fmt, ## arg);	\
	} while (0)

	#define PRINT_DBG(fmt, arg...) 			\
	do { 								\
		if(app_dbg_level > 2)		\
			printf(fmt, ## arg);	\
	} while (0)

	#define PRINT(fmt, arg...) 			\
	do { 								\
		if(app_dbg_level > 200)		\
			printf(fmt, ## arg);	\
	} while (0)

	// INFO level = 3生效
	#define LOG_I(fmt, arg...) 			\
	do { 								\
			if ((app_dbg_level%10) == 3)		\
				printf("[LOG_I]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	 } while (0)

	//方便打印后台信息
	#define DBG_BKG(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x01)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	} while (0)

	//方便打印QQ物联处理逻辑信息
	#define DBG_SDK(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x02)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	} while (0)

    //方便打印SQLite信息
	#define DBG_SQL(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x04)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	} while (0)

	//方便打印MJ信息
	#define DBG_YKT(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x08)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	} while (0)

	#define PRINT_MJ(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x08)		\
			printf(fmt, ## arg);	\
	} while (0)

	#define DBG_UDP(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x10)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	} while (0)

	#define PRINT_UDP(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x10)		\
			printf(fmt, ## arg);	\
	} while (0)

	//方便打印Cfg信息
	#define DBG_CFG(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x20)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	} while (0)

	#define DBG_TCP(fmt, arg...) 			\
	do { 								\
		if ((app_dbg_level / 10) & 0x30)		\
			printf("[DEBUG]\t%s:%d " fmt,  __FILE__, __LINE__, ## arg); 	\
	} while (0)
	#define DBG_CARD(fmt, arg...) 			\
	do { 							\
		if (card_dbg_level > 1)		\
				printf("[CARD]\t%s:%s:%d " fmt,  __FILE__, __FUNCTION__, __LINE__, ## arg); 	\
		else if(card_dbg_level > 0)		\
				printf("[CARD]\t" fmt, ## arg);	\
	} while (0)
#else
	#define PRINT(fmt, arg...) 			\
	do { 								\
		if(app_dbg_level > 200)		\
			printf(fmt, ## arg);	\
	} while (0)

	#include "Logging.h"
    //为了屏蔽所有打印,只打印该等级信息.方便独立调试功能.
	#define DBG(fmt, arg...)    printf("%s:%s:%d:" fmt,   __FILE__, __func__, __LINE__,  ## arg);

	#define DEBUG(...)    app_logging_fn(APP_LOG_DBG, __FILE__, __LINE__, __func__, __VA_ARGS__)
	#define DEBUG_COLOR(...)    app_log_out(FG_BLUE, APP_LOG_DBG,   __FILE__, __LINE__, __func__, __VA_ARGS__)

	//方便打印后台信息
	#define DBG_BKG(...)  app_logging_fn(APP_LOG_BKG, __FILE__, __LINE__, __func__, __VA_ARGS__)
	//方便打印系统信息
	#define DBG_SYS(...)  app_logging_fn(APP_LOG_SYS, __FILE__, __LINE__, __func__, __VA_ARGS__)

    //方便打印SQLite信息
	#define DBG_SQL(...)  app_logging_fn(APP_LOG_SQL, __FILE__, __LINE__, __func__, __VA_ARGS__)

	//方便打印YKT信息
	#define DBG_YKT(...)   app_logging_fn(APP_LOG_YKT, __FILE__, __LINE__, __func__, __VA_ARGS__)


	#define DBG_UDP(...)  app_logging_fn(APP_LOG_UDP, __FILE__, __LINE__, __func__, __VA_ARGS__)
	#define DBG_TCP(...)  app_logging_fn(APP_LOG_TCP, __FILE__, __LINE__, __func__, __VA_ARGS__)
	//方便打印Cfg信息
	#define DBG_CFG(...) 	app_logging_fn(APP_LOG_CFG, __FILE__, __LINE__, __func__, __VA_ARGS__)
	#define DBG_THD(...)    app_logging_fn(APP_LOG_THD, __FILE__, __LINE__, __func__, __VA_ARGS__)
	#define DBG_UART(...)   app_logging_fn(APP_LOG_UART, __FILE__, __LINE__, __func__, __VA_ARGS__)
	#define DBG_CARD(...)   app_logging_fn(APP_LOG_CARD, __FILE__, __LINE__, __func__, __VA_ARGS__)
    #define DBG_POOL(...)   app_logging_fn(APP_LOG_POOL, __FILE__, __LINE__, __func__, __VA_ARGS__)
#endif


#else
	#define DEBUGA(format, ...)
	#define DEBUGA(format, ...)
#endif


#ifdef __DEBUG__
	//#define  PAUSE() pause()
	#define  PAUSE() getchar()
#else
	#define  PAUSE()
#endif




#define TCP_DBG                         //定义:打开TcpSvr调试功能;未定义:关闭TcpSvr调试功能

#endif /* INC_BUILDCONFIG_H_ */
