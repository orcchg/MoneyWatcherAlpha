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
  : iDatabase(i_db_name)
  , m_next_id(0)
  , m_table_name("Daily_Table") {
  INF("enter DailyTable constructor.");
  this->__init__(this->m_table_name);
  ++DailyTable::OPENED_DAILY_TABLES_COUNT;
  INF("exit DailyTable constructor.");
}

DailyTable::~DailyTable() {
  INF("enter DailyTable destructor.");
  this->__close_database__();
  --DailyTable::OPENED_DAILY_TABLES_COUNT;
  INF("exit DailyTable destructor.");
}

Record DailyTable::addRecord(
    const MoneyValue_t& i_balance,
    const std::wstring& i_description,
    const Status& i_status) {
  INF("enter DailyTable::addRecord().");
  std::string insert_statement = "INSERT INTO \'";
  insert_statement += this->m_table_name;
  insert_statement += "\' VALUES(?1, ?2, ?3, ?4, ?5, ?6);";
  int nByte = static_cast<int>(insert_statement.length());
  TRC("Provided string SQL statement: "%s" of length %i.", insert_statement.c_str(), nByte);
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
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  bool accumulate = true;
  ID_t record_id = this->m_next_id++;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, record_id) == SQLITE_OK);
  DBG("ID [%lli] has been stored in SQLite database "%s".",
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
  DBG("Date ["%s"] has been stored in SQLite database "%s".",
      date.c_str(), this->m_db_name.c_str());
  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          3,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Time ["%s"] has been stored in SQLite database "%s".",
      time.c_str(), this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 4, i_balance) == SQLITE_OK);
  DBG("Balance [%lli] has been stored in SQLite database "%s".",
      i_balance, this->m_db_name.c_str());
  int description_n_bytes = i_description.length() * sizeof(wchar_t);
  accumulate = accumulate &&
      (sqlite3_bind_text16(
          this->m_db_statement,
          5,
          i_description.c_str(),
          description_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Description ["%ls"] has been stored in SQLite database "%s".",
      i_description.c_str(), this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 6, static_cast<sqlite3_int64>(i_status)) == SQLITE_OK);
  DBG("Status [%lli] has been stored in SQLite database "%s".",
	    static_cast<sqlite3_int64>(i_status), this->m_db_name.c_str());
  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into database "%s" by statement "%s"!",
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
  INF("exit DailyTable::addRecord().");
  return (record);
}

Record DailyTable::readRecord(const ID_t& i_record_id) {
  INF("enter DailyTable::readRecord().");
  std::string select_statement = "SELECT * FROM \'";
  select_statement += this->m_table_name;
  select_statement += "\' WHERE ID == \'";
  select_statement += std::to_string(i_record_id);
  select_statement += "\';";
  int nByte = static_cast<int>(select_statement.length());
  TRC("Provided string SQL statement: "%s" of length %i.", select_statement.c_str(), nByte);
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
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
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
  DBG("Loaded column data: Date ["%s"]; Time ["%s"]; Balance [%lli]; Description ["%ls"]; Status [%lli].",
	    datetime.getDate().c_str(), datetime.getTime().c_str(), balance, description.c_str(), raw_status);
  Record record(id, balance, description, status, datetime);
  DBG("Proper record instance has been constructed.");
#if ENABLED_DB_CACHING
  // TODO: caching the record
#endif
  this->__finalize__(select_statement.c_str());
  INF("exit DailyTable::readRecord().");
  return (record);
}

const std::string& DailyTable::getName() const {
  INF("enter DailyTable::getName().");
  DBG("Return the name "%s" of DailyTable at %p.", this->m_db_name.c_str(), this);
  INF("exit DailyTable::getName().");
  return (this->m_db_name);
}

bool DailyTable::load() {
  INF("enter DailyTable::load().");
#if ENABLED_DB_CACHING
  // TODO: caching the records
#endif
  INF("exit DailyTable::load().");
  return (false);
}


/* Private members */
// ----------------------------------------------------------------------------
void DailyTable::__init__(const std::string& i_table_name) {
  INF("enter DailyTable::__init__().");
  iDatabase::__init__(i_table_name);
  INF("exit DailyTable::__init__().");
}

void DailyTable::__open_database__() {
  INF("enter DailyTable::__open_database__().");
  iDatabase::__open_database__();
  INF("exit DailyTable::__open_database__().");
}

void DailyTable::__close_database__() {
  INF("enter DailyTable::__close_database__().");
  iDatabase::__close_database__();
  INF("exit DailyTable::__close_database__().");
}

void DailyTable::__create_table__(const std::string& i_table_name) {
  INF("enter DailyTable::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += i_table_name;
  statement += "('ID' INTEGER PRIMARY KEY, "
      "'Date' TEXT, "
      "'Time' TEXT, "
      "'Balance' INTEGER, "
      "'Description' TEXT, "
      "'Status' INTEGER);";
  int nByte = static_cast<int>(statement.length());
  TRC("Provided string SQL statement: "%s" of length %i.", statement.c_str(), nByte);
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
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  DBG("Table "%s" has been successfully created.", i_table_name.c_str());
  this->__finalize__(statement.c_str());
  INF("exit DailyTable::__create_table__().");
}

bool DailyTable::__does_table_exist__(const std::string& i_table_name) {
  INF("enter DailyTable::__does_table_exist__().");
  return (iDatabase::__does_table_exist__(i_table_name));
}

void DailyTable::__terminate__(const char* i_message) {
  INF("enter DailyTable::__terminate__().");
  iDatabase::__terminate__(i_message);
  INF("exit DailyTable::__terminate__().");
}

void DailyTable::__finalize__(const char* i_statement) {
  INF("enter DailyTable::__finalize__().");
  iDatabase::__finalize__(i_statement);
  INF("exit DailyTable::__finalize__().");
}

void DailyTable::__finalize_and_throw__(const char* i_statement) {
  INF("enter DailyTable::__finalize_and_throw__().");
  iDatabase::__finalize_and_throw__(i_statement);
}

}  /* namespace mw */
