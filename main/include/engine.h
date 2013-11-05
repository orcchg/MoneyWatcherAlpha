/*
 * engine.h
 *
 *  Description: Main MoneyWatcher engine declaration.
 *
 *  Created on: Nov 5, 2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <QtWidgets/QApplication>


namespace mw {

/// @class MoneyWatcherEngine
class MoneyWatcherEngine {
public:
  MoneyWatcherEngine(int argc, char* argv[]);
  virtual ~MoneyWatcherEngine();

  /// @brief Run Graphic application.
  void run();

private:
  QApplication m_application;

  MoneyWatcherEngine(const MoneyWatcherEngine& obj) = delete;
  MoneyWatcherEngine(MoneyWatcherEngine&& rval_obj) = delete;
  MoneyWatcherEngine& operator = (const MoneyWatcherEngine& rhs) = delete;
  MoneyWatcherEngine& operator = (MoneyWatcherEngine&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* ENGINE_H_ */
