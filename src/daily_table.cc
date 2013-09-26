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

DailyTable::DailyTable(const std::string& i_db_name)
  : m_db_name(i_db_name)
  , m_db_handler(nullptr)
  , m_db_statement(nullptr) {
  DBG("enter DailyTable constructor.");
  this->__open_database__();
  try {
    this->__create_table__();
  } catch(DailyTableException& e) {
    ERR(e.what());
    this->__terminate__("Error during create table.");
    // Do not allow invalid object of DailyTable to be instantiated.
    throw DailyTableException("Creating database has failed!");
  }
  DBG("exit DailyTable constructor.");
}

DailyTable::~DailyTable() {
  DBG("enter DailyTable destructor.");
  this->__close_database__();
  DBG("exit DailyTable destructor.");
}

Record DailyTable::addRecord(
    const MoneyValue_t& i_balance,
    const std::wstring& i_description,
    const Status& i_status) {
  DBG("enter DailyTable::addRecord().");
  const char* insert_statement = "INSERT INTO Daily_Table"
      " VALUES(?1, ?2, ?3, ?4, ?5, ?6);";
  int nByte = static_cast<int>(strlen(insert_statement));
  DBG("Provided string SQL statement: \"%s\" of length %i.", statement, nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  int result = sqlite3_prepare_v2(
        this->m_db_handler,
        insert_statement,
        nByte,
        &(this->m_db_statement),
        nullptr);
  if (result != SQLITE_OK) {
    ERR("Unable to prepare statement \"%s\"!", insert_statement);
    sqlite3_finalize(this->m_db_statement);
    this->m_db_statement = nullptr;
    TRC("Statement \"%s\" has been finalized.", insert_statement);
    throw DailyTableException("Unable to prepare statement \"%s\"!", insert_statement);
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      insert_statement_handler);
  bool accumulate = true;
  ID_t record_id = this->next_id++;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, record_id) == SQLITE_OK);
  DBG("ID [%lli] has been stored in SQLite database \"%s\".",
      record_id, this->m_db_name.c_str());
  DateTime current_datetime;
  std::string date = current_datetime.getDate();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          2,
          date.c_str(),
          date.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Date [\"%s\"] has been stored in SQLite database \"%s\".",
      date.c_str(), this->m_db_name.c_str());
  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          3,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Time [\"%s\"] has been stored in SQLite database \"%s\".",
      time.c_str(), this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 4, i_balance) == SQLITE_OK);
  DBG("Balance [%lli] has been stored in SQLite database \"%s\".",
      balance, this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_text16(
          this->m_db_statement,
          5,
          i_description.c_str(),
          i_description.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Description [\"%s\"] has been stored in SQLite database \"%s\".",
      description.c_str(), this->m_db_name.c_str());
  /*accumulate = accumulate &&
      (sqlite3_bind_int(this->m_db_statement, 6, i_status.serialize()) == SQLITE_OK);
  DBG("Status [\"%s\"] has been stored in SQLite database \"%s\".",
      <string representation of status>, this->m_db_name.c_str());*/
  // TODO: store Status as integer
  if (!accumulate) {
    ERR("Error during saving data into database \"%s\" by statement \"%s\"!",
        this->m_db_name.c_str(), insert_statement);
    sqlite3_finalize(this->m_db_statement);
    this->m_db_statement = nullptr;
    TRC("Statement \"%s\" has been finalized.", insert_statement);
    throw DailyTableException("Unable to insert data by statement \"%s\"!",
                              insert_statement);
  } else {
    DBG("All insertions have succeeded.");
  }
  // TODO: caching the record ???
  sqlite3_finalize(this->m_db_statement);
  this->m_db_statement = nullptr;
  TRC("Statement \"%s\" has been finalized.", insert_statement);
  return (Record(i_balance, i_description, i_status));
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
    throw DailyTableException("Unable to open database \"%s\"!",
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
    DBG("Database \"%s\" has been already shut down.", this->m_db_name.c_str());
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
    throw DailyTableException("Unable to prepare statement \"%s\"!", statement);
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  DBG("Table \"%s\" has been successfully created.", "Daily_Table");
  sqlite3_finalize(this->m_db_statement);
  this->m_db_statement = nullptr;
  TRC("Statement \"%s\" has been finalized.", statement);
  DBG("exit DailyTable::__create_table__().");
}

void DailyTable::__terminate__(const char* i_message) {
  DBG("enter DailyTable::__terminate__().");
  WRN(i_message);
  sqlite3_close(this->m_db_handler);
  this->m_db_handler = nullptr;
  TRC("Database \"%s\" has been shut down.", this->m_db_name.c_str());
  sqlite3_free(nullptr);
  DBG("exit DailyTable::__terminate__().");
}

/* Exception class implementation */
// ----------------------------------------------------------------------------
DailyTableException::DailyTableException(const char* i_message)
  : m_message(i_message) {
}

DailyTableException::~DailyTableException() {
}

const char* DailyTableException::what() const throw() {
  return (this->m_message);
}

}  /* namespace mw */
