/*
 * daily_table.cc
 *
 *  Description: Daily Table class implementation.
 *
 *  Created on: 14.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <stdexcept>
#include "daily_table.h"
#include "logger.h"
#include "sqlite3.h"


namespace mw {

DailyTable::DailyTable(const std::string& table_name)
  : m_table_name(table_name)
  , m_db_handler(nullptr) {
  DBG("enter DailyTable constructor.");
  this->__open_database__();
  DBG("exit DailyTable constructor.");
}

DailyTable::~DailyTable() {
  DBG("enter DailyTable destructor.");
  this->__close_database__();
  DBG("exit DailyTable destructor.");
}

void DailyTable::addRecord(
    const std::wstring& name,
    const MoneyValue_t& balance,
    const std::wstring& description,
    const Status& status) {
  DBG("enter DailyTable::addRecord().");
  //
  DBG("exit DailyTable::addRecord().");
}

/* Private members */
// ----------------------------------------------------------------------------
void DailyTable::__open_database__() {
  DBG("enter DailyTable::__open_database__().");
  int result = sqlite3_open(this->m_table_name.c_str(), &(this->m_db_handler));
  if (result != SQLITE_OK) {
    ERR("Unable to open database %s!", this->m_table_name.c_str());
    sqlite3_close(this->m_db_handler);
    TRC("Database %s has been shut down.", this->m_table_name.c_str());
    throw std::runtime_error("Unable to open database!");
  }
  DBG("exit DailyTable::__open_database__().");
}

void DailyTable::__close_database__() {
  DBG("enter DailyTable::__close_database__().");
  if (this->m_db_handler) {
    DBG("Found valid database handler.");
    sqlite3_close(this->m_db_handler);
    DBG("Database %s has been successfully closed.", this->m_table_name.c_str());
  }
  DBG("exit DailyTable::__close_database__().");
}

void DailyTable::__create_table__() {
  DBG("enter DailyTable::__create_table__().");
  const char* statement = "CREATE TABLE Daily_Table("
      "'ID' INTEGER PRIMARY KEY,"
      "'Date' INTEGER,"
      "'Time' INTEGER,"
      "'Balance' INTEGER,"
      "'Description' TEXT);";
  DBG("exit DailyTable::__create_table__().");
}

}  /* namespace mw */
