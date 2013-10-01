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

#define PROMPT_SUGGEST " >>> "
#define CRT(fmt, ...) printf((/*"CRT" PROMPT_SUGGEST "  " */fmt "\n"), ##__VA_ARGS__)
#define ERR(fmt, ...) printf((/*"ERR" PROMPT_SUGGEST "  " */fmt "\n"), ##__VA_ARGS__)
#define WRN(fmt, ...) printf((/*"WRN" PROMPT_SUGGEST "  " */fmt "\n"), ##__VA_ARGS__)
#define INF(fmt, ...) printf((/*"INF" PROMPT_SUGGEST "  " */fmt "\n"), ##__VA_ARGS__)
#define DBG(fmt, ...) printf((/*"DBG" PROMPT_SUGGEST "  " */fmt "\n"), ##__VA_ARGS__)
#define TRC(fmt, ...) printf((/*"TRC" PROMPT_SUGGEST "  " */fmt "\n"), ##__VA_ARGS__)

#else

#define CRT(fmt, ...)
#define ERR(fmt, ...)
#define WRN(fmt, ...)
#define INF(fmt, ...)
#define DBG(fmt, ...)
#define TRC(fmt, ...)

#endif

namespace mw {

class Logger {
public:
  Logger();
  virtual ~Logger();

private:
  void initialize();
  void deinitialize();
};

}  /* namespace mw */

#endif /* LOGGER_H_ */
