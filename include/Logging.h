/*
 * Logging.h
 *
 *  Created on: 2017年6月8日
 *      Author: zzh
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#ifdef __cplusplus
	extern "C" {
#endif

//#define APP_LOG_SIMPLIFY_FILE_NAME

typedef enum {
	APP_LOG_DBG      = 1,
	APP_LOG_VERBOSE  = 1<<1,     //2-
	APP_LOG_BKG      = 1<<2,	 //4-打印与后台通讯信息
	APP_LOG_SYS      = 1<<3,  	 //8-打印系统信息
	APP_LOG_SQL      = 1<<4,     //16-打印SQLite信息
	APP_LOG_YKT      = 1<<5,     //32-打印与一卡通服务器通讯信息
	APP_LOG_UDP      = 1<<6,     //64-打印udp通讯相关信息
	APP_LOG_TCP      = 1<<7,     //128-打印tcp通讯相关信息
	APP_LOG_SIP      = 1<<8,	 //256-打印sip通讯相关信息
	APP_LOG_UART     = 1<<9,     //512-打印uart通讯相关信息
	APP_LOG_THD      = 1<<10,	 //1024-打印线程通讯相关信息
	APP_LOG_CFG      = 1<<11,    //2048-打印配置文件相关信息
	APP_LOG_POOL     = 1<<12,    //4096-打印线程池相关信息

	APP_LOG_CARD     = 1<<16,
}AppLogType_e;


const typedef enum _ForeColor_e
{
	FG_DEFAULT      = -1,
	FG_BLACK		= 30, //黑色
	FG_RED			= 31, //红色
	FG_GREEN		= 32, //绿色
	FG_YELLOW		= 33, //黄色
	FG_BLUE			= 34, //蓝色
	FG_PURPLE		= 35, //紫色
	FG_DARKGREEN	= 36, //深绿色
	FG_WHITE		= 37, //白色
}ForeColor_e;

const typedef enum _BackColor_e
{
	BG_DEFAULT      = -1,
	BG_BLACK		= 40, //黑色
	BG_DARKRED		= 41, //深红色
	BG_GREEN		= 42, //绿色
	BG_YELLOW		= 43, //黄色
	BG_BLUE			= 44, //蓝色
	BG_PURPLE		= 45, //紫色
	BG_DARKGREEN	= 46, //深绿色
	BG_WHITE		= 47, //白色
}BackColor_e;

#define app_log_level_enabled(level)	(app_get_log_level_mask() & (level))
void app_set_log_level_mask(int levelmask);
int app_get_log_level_mask(void);

void app_log_out(ForeColor_e fc,\
		         AppLogType_e level, char const *file, int const line, char const *fn, const char *format, ...);
void app_logging_fn(AppLogType_e level, char const *file, int const line, char const *fn, const char *format, ...);



#ifdef __cplusplus
}
#endif

#endif /* LOGGING_H_ */
