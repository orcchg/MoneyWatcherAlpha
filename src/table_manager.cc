/*
 * table_manager.cc
 *
 *  Description: TableManager class implementation.
 *
 *  Created on: 12.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "logger.h"
#include "table_manager.h"


namespace mw {

int TableManager::OPENED_DATABASES_COUNT = 0;
const std::string TableManager::single_database_name = "MW-Database.db";

TableManager::TableManager()
  : m_cycle_table(TableManager::single_database_name)
  , m_daily_table(TableManager::single_database_name) {
  INF("enter TableManager constructor.");
  ++TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager constructor.");
}

TableManager::~TableManager() {
  INF("enter TableManager destructor.");
  --TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager destructor.");
}

}  /* namespace mw */
