/*
 * daily_table.cc
 *
 *  Description: Daily Table class implementation.
 *
 *  Created on: 14.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <stdexcept>
#include <cstring>
#include "daily_table.h"
#include "logger.h"
#include "sqlite3.h"


namespace mw {

DailyTable::DailyTable(const std::string& db_name)
  : m_db_name(db_name)
  , m_db_handler(nullptr)
  , m_db_statement(nullptr) {
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
  int result = sqlite3_open(this->m_db_name.c_str(), &(this->m_db_handler));
  if (result != SQLITE_OK) {
    ERR("Unable to open database \"%s\"!", this->m_db_name.c_str());
    sqlite3_close(this->m_db_handler);
    this->m_db_handler = nullptr;
    TRC("Database \"%s\" has been shut down.", this->m_db_name.c_str());
    throw std::runtime_error("Unable to open database \"%s\"!",
                             this->m_db_name.c_str());
  }
  DBG("SQLite database has been successfully opened and placed into %p.",
      this->m_db_handler);
  DBG("exit DailyTable::__open_database__().");
}

void DailyTable::__close_database__() {
  DBG("enter DailyTable::__close_database__().");
  if (this->m_db_handler) {
    DBG("Found valid database handler.");
    sqlite3_close(this->m_db_handler);
    this->m_db_handler = nullptr;
    DBG("Database \"%s\" has been successfully closed.",
        this->m_db_name.c_str());
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
  int nByte = static_cast<int>(strlen(statement));
  DBG("Provided string SQL statement: \"%s\" of length %i.", statement, nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      statement,
      nByte,
      &(this->m_db_statement),
      nullptr);
  if (result != SQLITE_OK) {
    ERR("Unable to prepare statement \"%s\"!", statement);
    sqlite3_finalize(this->m_db_statement);
    TRC("Statement \"%s\" has been finalized.", statement);
    throw std::runtime_error("Unable to prepare statement \"%s\"!", statement);
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  //
  DBG("exit DailyTable::__create_table__().");
}

void DailyTable::__terminate__(const char* message) {
  DBG("enter DailyTable::__terminate__().");
}

}  /* namespace mw */
