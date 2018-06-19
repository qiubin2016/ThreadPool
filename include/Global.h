/*
 * Global.h
 *
 *  Created on: 2016年8月23日
 *      Author: zzh
 */

#ifndef INCLUDE_GLOBAL_H_
#define INCLUDE_GLOBAL_H_

#define G_UNUSED(x)   (void)x;
#define CLEAR(x)      memset (&(x), 0, sizeof(x))
#define CLEAR_ARRAY(x) memset(&(x[0]), 0, sizeof(x))
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define MIN(__a, __b)   (__b < __a ? __b : __a)
#define MAX(__a, __b)   (__b < __a ? __a : __b)

#endif /* INCLUDE_GLOBAL_H_ */
