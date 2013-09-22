/*
 * logger.h
 *
 *  Description: Logger functions declaration.
 *
 *  Created on: 22.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#define ENABLE_LOGGING 1

#if ENABLE_LOGGING
#include <stdio.h>
#include <stdarg.h>

#define CRT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define ERR(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define WRN(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define INF(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define DBG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define TRC(fmt, ...) printf(fmt, ##__VA_ARGS__)

#else

#define CRT(fmt, ...)
#define ERR(fmt, ...)
#define WRN(fmt, ...)
#define INF(fmt, ...)
#define DBG(fmt, ...)
#define TRC(fmt, ...)

#endif

#endif /* LOGGER_H_ */
