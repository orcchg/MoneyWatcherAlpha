/*
 * engine.h
 *
 *  Description: Console application engine declaration.
 *
 *  Created on: 19.01.2014
 *      Author: Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "table_manager.h"


namespace mw {

class ConsoleEngine {
public:
  ConsoleEngine();
  virtual ~ConsoleEngine();

  void run();

private:
  TableManager tm;
};

}  /* namespace mw */

#endif /* ENGINE_H_ */
