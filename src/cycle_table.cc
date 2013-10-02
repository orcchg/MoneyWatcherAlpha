/*
 * cycle_table.cc
 *
 *  Description: Cycle Table class implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "cycle_table.h"
#include "logger.h"


namespace mw {

int CycleTable::OPENED_CYCLE_TABLES_COUNT = 0;

/* Private members */
// ----------------------------------------------------------------------------
void CycleTable::__open_database__() {
  DBG("enter CycleTable::__open_database__().");
  iDatabase::__open_database__();
  DBG("exit CycleTable::__open_database__().");
}

void CycleTable::__close_database__() {
  DBG("enter CycleTable::__close_database__().");
  iDatabase::__close_database__();
  DBG("exit CycleTable::__close_database__().");
}

void CycleTable::__create_table__(const std::string& i_table_name) {
  DBG("enter CycleTable::__create_table__().");
  iDatabase::__create_table__(i_table_name);
  DBG("exit CycleTable::__create_table__().");
}

bool CycleTable::__does_table_exist__(const std::string& i_table_name) {
  DBG("enter CycleTable::__does_table_exist__().");
  return (iDatabase::__does_table_exist__(i_table_name));
}

void CycleTable::__terminate__(const char* i_message) {
  DBG("enter CycleTable::__terminate__().");
  iDatabase::__terminate__(i_message);
  DBG("exit CycleTable::__terminate__().");
}

void CycleTable::__finalize__(const char* i_statement) {
  DBG("enter CycleTable::__finalize__().");
  iDatabase::__finalize__(i_statement);
  DBG("exit CycleTable::__finalize__().");
}

void CycleTable::__finalize_and_throw__(const char* i_statement) {
  DBG("enter CycleTable::__finalize_and_throw__().");
  iDatabase::__finalize_and_throw__(i_statement);
}

}  /* namespace mw */
