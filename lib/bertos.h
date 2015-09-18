// This file is modified from BeRTOS.

#ifndef BERTOS_H
#define BERTOS_H

#include <stdbool.h>
#include <stdint.h>

//typedef uint32_t size_t;
typedef uint32_t id_t;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define EOF   (-1)


#define INLINE static __INLINE

#define DB(x) x
#define BV(x)  (1<<(x))

#define LIKELY(x)               __builtin_expect(!!(x), 1)
#define UNLIKELY(x)             __builtin_expect(!!(x), 0)

#define PGM_FUNC(x)      x

#ifndef MIN
#define MIN(a,b) ((a < b) ? a : b) 
#endif

#define DIV_ROUND(dividend, divisor)  (((dividend) + (divisor) / 2) / (divisor))

#define MAKE_ID(a,b,c,d) \
	( ((uint32_t)(a) << 24) \
	| ((uint32_t)(b) << 16) \
	| ((uint32_t)(c) <<  8) \
	| ((uint32_t)(d) <<  0) )

#define countof(a)  (sizeof(a) / sizeof(*(a)))

#endif
