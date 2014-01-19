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
  ConsoleEngine() = default;
  virtual ~ConsoleEngine() = default;

  void run();

private:
  TableManager m_table_manager;

  void __banner__();
  void __show_menu__();
  bool __list_cycle_table__();
  void __list_daily_table__();
  void __run_cycle_table_operations__();

  ConsoleEngine(const ConsoleEngine& obj) = delete;
  ConsoleEngine(ConsoleEngine&& rval_obj) = delete;
  ConsoleEngine& operator = (const ConsoleEngine& rhs) = delete;
  ConsoleEngine& operator = (ConsoleEngine&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* ENGINE_H_ */
