/*
 * engine.cc
 *
 *  Description: Main MoneyWatcher engine implementation.
 *
 *  Created on: Nov 5, 2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "engine.h"


namespace mw {

MoneyWatcherEngine::MoneyWatcherEngine(int argc, char* argv[])
  : m_application(argc, argv) {
}

MoneyWatcherEngine::~MoneyWatcherEngine() {
}

}  /* namespace mw */
