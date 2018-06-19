 
#ifndef  INCLUDE_H
#define  INCLUDE_H

#ifdef __cplusplus
	extern "C" { 
#endif
// https://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/Common-Predefined-Macros.html#Common%20Predefined%20Macros
#define GCC_VERSION (__GNUC__ * 10000  + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <unistd.h>
#include <sys/cdefs.h>
#include <linux/input.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sys/types.h>

#include <limits.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/watchdog.h>
#include <linux/sockios.h>
#include <linux/serial.h>
#include <math.h>

#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include <stdint.h>        /* Definition of uint64_t */
#include <netdb.h>

#include <net/if.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <assert.h>

#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <termios.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <crypt.h>
#include <shadow.h>

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/cdefs.h>



#include "BuildConfig.h"
#include "DataType.h"
#include "Global.h"
#include "comdef.h"

extern struct timeval g_time;
void PRINT_TIME(int);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
//#include "Cpp.h"
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#if (__cplusplus >= 201103L) &&  (GCC_VERSION > 40801)
#include <random>
#endif
#include <algorithm>
#include <bitset>

using namespace std;
#endif

#endif // INCLUDE_H
