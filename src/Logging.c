/*
 * Logging.c
 *
 *  Created on: 2017年6月8日
 *      Author: zzh
 */

#include "Include.h"
#include "Logging.h"

static unsigned int app_log_mask  = 0;

void app_set_log_level_mask(int levelmask)
{
	app_log_mask = levelmask;
	if(levelmask >= 2)
	{
		app_log_mask = levelmask | 0x03;
	}


}

int app_get_log_level_mask(void)
{
	return app_log_mask;
}

void app_log_out(ForeColor_e fc, \
		         AppLogType_e level, char const *file, int const line, char const *fn, const char *format, ...)
{
	G_UNUSED(fn);

	if (app_log_level_enabled(level))
	{
		const char *lvlstr = "";
		switch (level)
		{
			case APP_LOG_DBG: lvlstr = "[DBG]"; break;
			case APP_LOG_VERBOSE: lvlstr = "[VERBOSE]"; break;
			case APP_LOG_BKG: lvlstr = "[BKG]"; break;
			case APP_LOG_SYS: lvlstr = "[SYS]"; break;
			case APP_LOG_SQL: lvlstr = "[SQL]"; break;
			case APP_LOG_YKT: lvlstr = "[YKT]"; break;
			case APP_LOG_UDP: lvlstr = "[UDP]"; break;
			case APP_LOG_TCP: lvlstr = "[TCP]"; break;
			case APP_LOG_CFG: lvlstr = "[CFG]"; break;
			case APP_LOG_SIP: lvlstr = "[SIP]"; break;
			case APP_LOG_UART: lvlstr = "[UART]"; break;
			case APP_LOG_THD: lvlstr = "[THD]"; break;
			case APP_LOG_POOL: lvlstr = "[POOL]"; break;
			default: lvlstr = "[OTH]"; break;
		}

		fprintf(stdout, "\e[%dm", fc); //

		if(app_get_log_level_mask() == APP_LOG_DBG)
		{
			fprintf(stdout, "%s ", lvlstr);
		}
		else
		{
		#ifdef APP_LOG_SIMPLIFY_FILE_NAME
			const char *p= &file[strlen(file)];
			while((*p != '/') && (p != file))
			{
				p--;
			}
			fprintf(stdout, "%s %s:%d ", lvlstr, ++p, line);
		#else
			fprintf(stdout, "%s %s:%d ", lvlstr, file, line);
		#endif
		}
		va_list args;
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);

		fprintf(stdout, "\e[0m");//关闭所有属性
	}

	return;
}


void app_logging_fn(AppLogType_e level, char const *file, int const line, char const *fn, const char *format, ...)
{
	G_UNUSED(fn);

	if (app_log_level_enabled(level))
	{
		const char *lvlstr = "";
		switch (level)
		{
			case APP_LOG_DBG: lvlstr = "[DBG]"; break;
			case APP_LOG_VERBOSE: lvlstr = "[VERBOSE]"; break;
			case APP_LOG_BKG: lvlstr = "[BKG]"; break;
			case APP_LOG_SYS: lvlstr = "[SYS]"; break;
			case APP_LOG_SQL: lvlstr = "[SQL]"; break;
			case APP_LOG_YKT: lvlstr = "[YKT]"; break;
			case APP_LOG_UDP: lvlstr = "[UDP]"; break;
			case APP_LOG_TCP: lvlstr = "[TCP]"; break;
			case APP_LOG_CFG: lvlstr = "[CFG]"; break;
			case APP_LOG_SIP: lvlstr = "[SIP]"; break;
			case APP_LOG_UART: lvlstr = "[UART]"; break;
			case APP_LOG_THD: lvlstr = "[THD]"; break;
			case APP_LOG_POOL: lvlstr = "[POOL]"; break;
			default: lvlstr = "[OTH]"; break;
		}

		if(app_get_log_level_mask() == APP_LOG_DBG)
		{
			fprintf(stdout, "%s ", lvlstr);
		}
		else
		{
		#ifdef APP_LOG_SIMPLIFY_FILE_NAME
			const char *p= &file[strlen(file)];
			while((*p != '/') && (p != file))
			{
				p--;
			}
			fprintf(stdout, "%s %s:%d ", lvlstr, ++p, line);
		#else
			fprintf(stdout, "%s %s:%d ", lvlstr, file, line);
		#endif
		}
		va_list args;
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
	}

	return;
}



