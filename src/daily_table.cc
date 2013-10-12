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
#include <utility>
#include "daily_table.h"
#include "logger.h"
#include "sqlite3.h"


namespace mw {

int DailyTable::OPENED_DAILY_TABLES_COUNT = 0;
const std::string DailyTable::last_row_id_table_name = "Last_Record_ID";

DailyTable::DailyTable(const std::string& i_db_name)
  : iDatabase(i_db_name)
  , m_table_name("Daily_Table") {
  INF("enter DailyTable constructor.");
  this->__init__(this->m_table_name);
  ++DailyTable::OPENED_DAILY_TABLES_COUNT;
  INF("exit DailyTable constructor.");
}

DailyTable::DailyTable(DailyTable&& rval_obj)
  : iDatabase(std::move(static_cast<iDatabase&>(rval_obj)))
  , m_table_name(rval_obj.m_table_name) {
  INF("enter DailyTable move constructor.");
  rval_obj.m_table_name = "";
  INF("exit DailyTable move constructor.");
}

DailyTable::~DailyTable() {
  INF("enter DailyTable destructor.");
  this->__close_database__();
  --DailyTable::OPENED_DAILY_TABLES_COUNT;
  INF("exit DailyTable destructor.");
}

Record DailyTable::addRecord(
    const MoneyValue_t& i_balance,
    const WrappedString& i_description,
    const Status& i_status) {
  INF("enter DailyTable::addRecord().");
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_table_name;
  insert_statement += "' VALUES(?1, ?2, ?3, ?4, ?5, ?6);";
  int nByte = static_cast<int>(insert_statement.length());
  TRC("Provided string SQL statement: "%s" of length %i.", insert_statement.c_str(), nByte);
  TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
               this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      insert_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  this->__set_last_statement__(insert_statement.c_str());
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(insert_statement.c_str(), result);
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
  int description_n_bytes = i_description.n_bytes();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          5,
          i_description.c_str(),
          description_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Description ["%s"] has been stored in SQLite database "%s".",
      i_description.c_str(), this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 6, static_cast<sqlite3_int64>(i_status)) == SQLITE_OK);
  DBG("Status [%lli] has been stored in SQLite database "%s".",
        static_cast<sqlite3_int64>(i_status), this->m_db_name.c_str());
  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into database "%s" by statement "%s"!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(insert_statement.c_str(), SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG("All insertions have succeeded.");
  }
#if ENABLED_DB_CACHING
  // TODO: caching the record
#endif
  this->__finalize__(insert_statement.c_str());
  this->__increment_rows__();
  this->__write_last_id__(DailyTable::last_row_id_table_name, record_id);
  Record record(record_id, i_balance, i_description, i_status, current_datetime);
  DBG("Constructed output record.");
  INF("exit DailyTable::addRecord().");
  return (record);
}

Record DailyTable::readRecord(const ID_t& i_record_id) {
  INF("enter DailyTable::readRecord().");
  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE ID == '";
  select_statement += std::to_string(i_record_id);
  select_statement += "';";
  int nByte = static_cast<int>(select_statement.length());
  TRC("Provided string SQL statement: "%s" of length %i.", select_statement.c_str(), nByte);
  TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
               this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      select_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  this->__set_last_statement__(select_statement.c_str());
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(select_statement.c_str(), result);
  }
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  ID_t id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG("Read id [%lli] from database, input id was [%lli].", id, i_record_id);
  TABLE_ASSERT("Input record id does not equal to primary key value from database!" &&
               id == i_record_id);
  std::string date(reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 1)));
  std::string time(reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 2)));
  DateTime datetime(date, time);
  MoneyValue_t balance = sqlite3_column_int64(this->m_db_statement, 3);
  const void* raw_description = sqlite3_column_text(this->m_db_statement, 4);
  WrappedString description(raw_description);
  sqlite3_int64 raw_status = sqlite3_column_int64(this->m_db_statement, 5);
  Status status(raw_status);
  DBG("Loaded column data: Date ["%s"]; Time ["%s"]; Balance [%lli]; Description ["%s"]; Status [%lli].",
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

void DailyTable::deleteRecord(const ID_t& i_record_id) {
  INF("enter DailyTable::deleteRecord().");
  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID == '";
  delete_statement += std::to_string(i_record_id);
  delete_statement += "';";
  int nByte = static_cast<int>(delete_statement.length());
  TRC("Provided string SQL statement: "%s" of length %i.", delete_statement.c_str(), nByte);
  TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
               this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      delete_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  this->__set_last_statement__(delete_statement.c_str());
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(delete_statement.c_str(), result);
  }
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  this->__decrement_rows__();
  if (i_record_id + 1 == this->m_next_id) {
    --this->m_next_id;
    DBG("Deleted last record. Next id value has been decremented.");
  }
  if (this->__empty__(this->m_table_name)) {
    this->m_next_id = 0;
  }
  INF("exit DailyTable::deleteRecord().");
}

const std::string& DailyTable::getName() const {
  INF("enter DailyTable::getName().");
  DBG("Return the name "%s" of DailyTable at %p.", this->m_table_name.c_str(), this);
  INF("exit DailyTable::getName().");
  return (this->m_table_name);
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
  DBG("enter DailyTable::__init__().");
  iDatabase::__init__(i_table_name);
  iDatabase::__create_table_for_last_id__(DailyTable::last_row_id_table_name);
  ID_t last_row_id = this->__read_last_id__(DailyTable::last_row_id_table_name);
  this->m_next_id = last_row_id == 0 ? 0 : last_row_id + 1;
  TRC("Initialization has completed: total rows [%i], last row id [%lli], next_id [%lli].",
      this->m_rows, last_row_id, this->m_next_id);
  DBG("exit DailyTable::__init__().");
}

void DailyTable::__create_table__(const std::string& i_table_name) {
  DBG("enter DailyTable::__create_table__().");
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
  TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
               this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  this->__set_last_statement__(statement.c_str());
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(statement.c_str(), result);
  }
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  DBG("Table "%s" has been successfully created.", i_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG("exit DailyTable::__create_table__().");
}

}  /* namespace mw */
