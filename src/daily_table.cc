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

DailyTable::next_id = 0;

DailyTable::DailyTable(const std::string& db_name)
  : m_db_name(db_name)
  , m_db_handler(nullptr)
  , m_db_statement(nullptr) {
  DBG("enter DailyTable constructor.");
  this->__open_database__();
  try {
    this->__create_table__();
  } catch(std::runtime_error& e) {
    ERR(e.what());
    this->__terminate__("Error during create table.");
  }
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
  const char* insert_statement = "INSERT INTO Daily_Table"
      " VALUES(?1, ?2, ?3, ?4, ?5, ?6);";
  int nByte = static_cast<int>(strlen(insert_statement));
  DBG("Provided string SQL statement: \"%s\" of length %i.", statement, nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  DB_Statement insert_statement_handler = nullptr;
  int result = sqlite3_prepare_v2(
        this->m_db_handler,
        insert_statement,
        nByte,
        &(insert_statement_handler),
        nullptr);
  if (result != SQLITE_OK) {
    ERR("Unable to prepare statement \"%s\"!", insert_statement);
    sqlite3_finalize(insert_statement_handler);
    insert_statement_handler = nullptr;
    TRC("Statement \"%s\" has been finalized.", insert_statement);
    throw std::runtime_error("Unable to prepare statement \"%s\"!", insert_statement);
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      insert_statement_handler);
  bool accumulate = true;
  accumulate = accumulate &&
      (sqlite3_bind_int64(insert_statement_handler, 1, this->next_id++) == SQLITE_OK);
  DateTime current_datetime;
  std::string date = current_datetime.getDate();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          insert_statement_handler,
          2,
          date.c_str(),
          date.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          insert_statement_handler,
          3,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  accumulate = accumulate &&
      (sqlite3_bind_int64(insert_statement_handler, 4, balance) == SQLITE_OK);
  accumulate = accumulate &&
      (sqlite3_bind_text16(
          insert_statement_handler,
          5,
          description.c_str(),
          description.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  /*accumulate = accumulate &&
      (sqlite3_bind_int(insert_statement_handler, 6, status) == SQLITE_OK);*/
  // TODO: store Status as integer
  // TODO: add more logs
  // TODO: add checking 'accumulate'
  // TODO: caching the record ???
  // TODO: finalize
  // TODO: close database ???
  DBG("exit DailyTable::addRecord().");
}

/* Private members */
// ----------------------------------------------------------------------------
void DailyTable::__open_database__() {
  DBG("enter DailyTable::__open_database__().");
  int result = sqlite3_open(this->m_db_name.c_str(), &(this->m_db_handler));
  if (result != SQLITE_OK) {
    ERR("Unable to open database \"%s\"!", this->m_db_name.c_str());
    this->__terminate__("Error during open database.");
    throw std::runtime_error("Unable to open database \"%s\"!",
                             this->m_db_name.c_str());
  }
  DBG("SQLite database has been successfully opened and placed into %p.",
      this->m_db_handler);
  DBG("exit DailyTable::__open_database__().");
}

void DailyTable::__close_database__() {
  DBG("enter DailyTable::__close_database__().");
  if (this->m_db_statement) {
    DBG("Found prepared SQL statement at %p.", this->m_db_statement);
    sqlite3_finalize(this->m_db_statement);
    this->m_db_statement = nullptr;
    DBG("Statement has been successfully finalized.");
  } else {
    DBG("Statement has been already finalized.");
  }
  if (this->m_db_handler) {
    DBG("Found valid database handler at %p.", this->m_db_handler);
    sqlite3_close(this->m_db_handler);
    this->m_db_handler = nullptr;
    DBG("Database \"%s\" has been successfully closed.",
        this->m_db_name.c_str());
  } else {
    DBG("Database \"%s\" has been already shut down.", this->m_db_name);
  }
  sqlite3_free(nullptr);
  DBG("exit DailyTable::__close_database__().");
}

void DailyTable::__create_table__() {
  DBG("enter DailyTable::__create_table__().");
  const char* statement = "CREATE TABLE Daily_Table("
      "'ID' INTEGER PRIMARY KEY,"
      "'Date' TEXT,"
      "'Time' TEXT,"
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
    this->m_db_statement = nullptr;
    TRC("Statement \"%s\" has been finalized.", statement);
    throw std::runtime_error("Unable to prepare statement \"%s\"!", statement);
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  DBG("Table \"%s\" has been successfully created.", "Daily_Table");
  // TODO: finalize ???
  DBG("exit DailyTable::__create_table__().");
}

void DailyTable::__terminate__(const char* message) {
  DBG("enter DailyTable::__terminate__().");
  WRN(message);
  sqlite3_close(this->m_db_handler);
  this->m_db_handler = nullptr;
  TRC("Database \"%s\" has been shut down.", this->m_db_name.c_str());
  sqlite3_free(nullptr);
  DBG("exit DailyTable::__terminate__().");
}

}  /* namespace mw */
