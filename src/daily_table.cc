/*
 * daily_table.cc
 *
 *  Description: Daily Table class implementation.
 *
 *  Created on: 14.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <cstring>
#include <stdexcept>
#include "daily_table.h"
#include "logger.h"
#include "sqlite3.h"


namespace mw {

int DailyTable::OPENED_DAILY_TABLES_COUNT = 0;

DailyTable::DailyTable(const std::string& i_db_name)
  : m_db_name(i_db_name)
  , m_db_handler(nullptr)
  , m_db_statement(nullptr)
  , m_next_record_id(0)
  , m_table_name("Daily_Table") {
  DBG("enter DailyTable constructor.");
  this->__open_database__();
  try {
    this->__create_table__();
  } catch(DailyTableException& e) {
    ERR(e.what());  // TODO: fix logs
    this->__terminate__("Error during create table.");
    // Do not allow invalid object of DailyTable to be instantiated.
    throw e;
  }
  ++DailyTable::OPENED_DAILY_TABLES_COUNT;
  DBG("exit DailyTable constructor.");
}

DailyTable::~DailyTable() {
  DBG("enter DailyTable destructor.");
  this->__close_database__();
  --DailyTable::OPENED_DAILY_TABLES_COUNT;
  DBG("exit DailyTable destructor.");
}

Record DailyTable::addRecord(
    const MoneyValue_t& i_balance,
    const std::wstring& i_description,
    const Status& i_status) {
  DBG("enter DailyTable::addRecord().");
  std::string insert_statement = "INSERT INTO \'";
  insert_statement += this->m_table_name;
  insert_statement += "\' VALUES(?1, ?2, ?3, ?4, ?5, ?6);";
  int nByte = static_cast<int>(insert_statement.length());
  DBG("Provided string SQL statement: \"%s\" of length %i.", insert_statement.c_str(), nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      insert_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(insert_statement.c_str());
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  bool accumulate = true;
  ID_t record_id = this->m_next_record_id++;
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
      i_balance, this->m_db_name.c_str());
  int description_n_bytes = i_description.length() * sizeof(wchar_t);
  accumulate = accumulate &&
      (sqlite3_bind_text16(
          this->m_db_statement,
          5,
          i_description.c_str(),
          description_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Description [\"%S\"] has been stored in SQLite database \"%s\".",
      i_description.c_str(), this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 6, static_cast<sqlite3_int64>(i_status)) == SQLITE_OK);
  DBG("Status [%lli] has been stored in SQLite database \"%s\".",
	  static_cast<sqlite3_int64>(i_status), this->m_db_name.c_str());
  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into database \"%s\" by statement \"%s\"!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(insert_statement.c_str());
  } else {
    DBG("All insertions have succeeded.");
  }
#if ENABLED_DB_CACHING
  // TODO: caching the record
#endif
  this->__finalize__(insert_statement.c_str());
  Record record(record_id, i_balance, i_description, i_status, current_datetime);
  DBG("Constructed output record.");
  DBG("exit DailyTable::addRecord().");
  return (record);
}

Record DailyTable::readRecord(const ID_t& i_record_id) {
  DBG("enter DailyTable::readRecord().");
  std::string select_statement = "SELECT * FROM \'";
  select_statement += this->m_table_name;
  select_statement += "\' WHERE ID == \'";
  select_statement += std::to_string(i_record_id);
  select_statement += "\';";
  int nByte = static_cast<int>(select_statement.length());
  DBG("Provided string SQL statement: \"%s\" of length %i.", select_statement.c_str(), nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      select_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(select_statement.c_str());
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  ID_t id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG("Read id [%lli] from database, input id was [%lli].", id, i_record_id);
  assert("Input record id does not equal to primary key value from database!" &&
         id == i_record_id);
  std::string date(reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 1)));
  std::string time(reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 2)));
  DateTime datetime(date, time);
  MoneyValue_t balance = sqlite3_column_int64(this->m_db_statement, 3);
  const void* raw_description = sqlite3_column_text16(this->m_db_statement, 4);
  std::wstring description(static_cast<const wchar_t*>(raw_description));
  sqlite3_int64 raw_status = sqlite3_column_int64(this->m_db_statement, 5);
  Status status(raw_status);
  DBG("Loaded column data: Date [\"%s\"]; Time [\"%s\"]; Balance [%lli]; Description [\"%S\"]; Status [%lli].",
	  datetime.getDate().c_str(), datetime.getTime().c_str(), balance, description.c_str(), raw_status);
  Record record(id, balance, description, status, datetime);
  DBG("Proper record instance has been constructed.");
#if ENABLED_DB_CACHING
  // TODO: caching the record
#endif
  this->__finalize__(select_statement.c_str());
  DBG("exit DailyTable::readRecord().");
  return (record);
}

const std::string& DailyTable::getName() const {
  DBG("enter DailyTable::getName().");
  DBG("Return the name \"%s\" of DailyTable at %p.", this->m_db_name.c_str(), this);
  DBG("exit DailyTable::getName().");
  return (this->m_db_name);
}

bool DailyTable::load() {
  DBG("enter DailyTable::load().");
#if ENABLED_DB_CACHING
  // TODO: caching the records
#endif
  DBG("exit DailyTable::load().");
  return (false);
}

/* Private members */
// ----------------------------------------------------------------------------
void DailyTable::__open_database__() {
  DBG("enter DailyTable::__open_database__().");
  int result = sqlite3_open(this->m_db_name.c_str(), &(this->m_db_handler));
  if (result != SQLITE_OK) {
    ERR("Unable to open database \"%s\"!", this->m_db_name.c_str());
    this->__terminate__("Error during open database.");
    throw DailyTableException("Unable to open database!");
  }
  DBG("SQLite database \"%s\" has been successfully opened and placed into %p.",
      this->m_db_name.c_str(), this->m_db_handler);
  DBG("exit DailyTable::__open_database__().");
}

void DailyTable::__close_database__() {
  DBG("enter DailyTable::__close_database__().");
  if (this->m_db_statement) {
    DBG("Found prepared SQL statement at %p.", this->m_db_statement);
    this->__finalize__("");
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
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += this->m_table_name;
  statement += "('ID' INTEGER PRIMARY KEY, "
      "'Date' TEXT, "
      "'Time' TEXT, "
      "'Balance' INTEGER, "
      "'Description' TEXT, "
      "'Status' INTEGER);";
  int nByte = static_cast<int>(statement.length());
  DBG("Provided string SQL statement: \"%s\" of length %i.", statement.c_str(), nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(statement.c_str());
  }
  DBG("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  DBG("Table \"%s\" has been successfully created.", this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG("exit DailyTable::__create_table__().");
}

bool DailyTable::__does_table_exist__() {
  DBG("enter DailyTable::__does_table_exist__().");
  std::string check_statement = "SELECT * FROM \'";
  check_statement += this->m_table_name;
  check_statement += "\';";
  int nByte = static_cast<int>(check_statement.length());
  DBG("Provided string SQL statement: \"%s\" of length %i.", check_statement.c_str(), nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      check_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(check_statement.c_str());
  bool table_exists = false;
  switch (result) {
    case SQLITE_OK:
      DBG("SQLite table \"%s\" already exists.", this->m_table_name.c_str());
      table_exists = true;
      break;
    default:
      DBG("SQLite table \"%s\" does not exist.", this->m_table_name.c_str());
      break;
  }
  DBG("exit DailyTable::__does_table_exist__().");
  return (table_exists);
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

void DailyTable::__finalize__(const char* statement) {
  DBG("enter DailyTable::__finalize__().");
  sqlite3_finalize(this->m_db_statement);
  this->m_db_statement = nullptr;
  TRC("Statement \"%s\" has been finalized.", statement);
  DBG("exit DailyTable::__finalize__().");
}

void DailyTable::__finalize_and_throw__(const char* statement) {
  DBG("enter DailyTable::__finalize_and_throw__().");
  ERR("Unable to prepare statement \"%s\"!", statement);
  this->__finalize__(statement);
  DBG("exit DailyTable::__finalize_and_throw__().");
  throw DailyTableException("Unable to prepare statement!");
}

/* Exception class implementation */
// ----------------------------------------------------------------------------
DailyTableException::DailyTableException(const char* i_message)
  : m_message(i_message) {
}

DailyTableException::~DailyTableException() throw() {
}

const char* DailyTableException::what() const throw() {
  return (this->m_message);
}

}  /* namespace mw */
