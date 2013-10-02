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

#include "common.h"

#if ENABLED_LOGGING
#include <stdio.h>
#include <stdarg.h>

/* Basic logging */
// ----------------------------------------------
#define PROMPT_SUGGEST "] >>> "
#define COLOR_OPEN "\e[5;"
#define COLOR_ENCLOSING "\e[m"
#define NEWLINE "\n"
#define WHITESPACE " "
#define COLON ":"
#define LINE "%i"
#define PROMPT_CLOSE COLOR_ENCLOSING NEWLINE

#define CRT_COLOR "01;31m"
#define ERR_COLOR "00;31m"
#define WRN_COLOR "00;33m"
#define INF_COLOR "00;32m"
#define DBG_COLOR "00;36m"
#define TRC_COLOR "00;37m"

#define CRT_STRING "  CRT  in ["
#define ERR_STRING "  ERR  in ["
#define WRN_STRING "  WRN  in ["
#define INF_STRING "  INF  in ["
#define DBG_STRING "  DBG  in ["
#define TRC_STRING "  TRC  in ["

#define CRT_SUGGEST COLOR_OPEN CRT_COLOR CRT_STRING __FILE__ COLON LINE PROMPT_SUGGEST
#define ERR_SUGGEST COLOR_OPEN ERR_COLOR ERR_STRING __FILE__ COLON LINE PROMPT_SUGGEST
#define WRN_SUGGEST COLOR_OPEN WRN_COLOR WRN_STRING __FILE__ COLON LINE PROMPT_SUGGEST
#define INF_SUGGEST COLOR_OPEN INF_COLOR INF_STRING __FILE__ COLON LINE PROMPT_SUGGEST
#define DBG_SUGGEST COLOR_OPEN DBG_COLOR DBG_STRING __FILE__ COLON LINE PROMPT_SUGGEST
#define TRC_SUGGEST COLOR_OPEN TRC_COLOR TRC_STRING __FILE__ COLON LINE PROMPT_SUGGEST

#define CRT(fmt, ...) printf((CRT_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
#define ERR(fmt, ...) printf((ERR_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
#define WRN(fmt, ...) printf((WRN_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
#define INF(fmt, ...) printf((INF_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
#define DBG(fmt, ...) printf((DBG_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
#define TRC(fmt, ...) printf((TRC_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)

#else

#define CRT(fmt, ...)
#define ERR(fmt, ...)
#define WRN(fmt, ...)
#define INF(fmt, ...)
#define DBG(fmt, ...)
#define TRC(fmt, ...)

#endif

namespace mw {

}  /* namespace mw */

#endif /* LOGGER_H_ */
