/*
 * daily_table.cc
 *
 *  Description: Daily Table class implementation.
 *
 *  Created on: 14.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <algorithm>
#include <utility>
#include "daily_table.h"
#include "logger.h"
#include "service.h"
#include "sqlite3.h"


namespace mw {

int DailyTable::OPENED_DAILY_TABLES_COUNT = 0;

DailyTable::DailyTable(
    const std::string& i_db_name,
    const std::string& i_table_name)
  : iDatabase(i_db_name, i_table_name) {
  INF("enter DailyTable constructor.");
  this->__init__();
  ++DailyTable::OPENED_DAILY_TABLES_COUNT;
  INF("exit DailyTable constructor.");
}

DailyTable::DailyTable(DailyTable&& rval_obj)
  : iDatabase(std::move(static_cast<iDatabase&>(rval_obj))) {
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
    const RecordStatus& i_status) {
  INF("enter DailyTable::addRecord().");
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_table_name;
  insert_statement += "' VALUES(?1, ?2, ?3, ?4, ?5, ?6);";
  this->__prepare_statement__(insert_statement);

  bool accumulate = true;
  ID_t record_id = this->m_next_id++;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, record_id) == SQLITE_OK);
  DBG1("ID [%lli] has been stored in table ["%s"], SQLite database ["%s"].",
       record_id, this->m_table_name.c_str(), this->m_db_name.c_str());

  DateTime current_datetime;
  std::string date = current_datetime.getDate();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          2,
          date.c_str(),
          date.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG1("Date ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
       date.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          3,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG1("Time ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
       time.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 4, i_balance) == SQLITE_OK);
  DBG1("Balance [%lli] has been stored in table ["%s"], "
       "SQLite database ["%s"].",
       i_balance, this->m_table_name.c_str(), this->m_db_name.c_str());

  int description_n_bytes = i_description.n_bytes();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          5,
          i_description.c_str(),
          description_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG1("Description ["%s"] has been stored in table ["%s"], "
       "SQLite database ["%s"].",
       i_description.c_str(),
       this->m_table_name.c_str(),
       this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          6,
          static_cast<sqlite3_int64>(i_status)) == SQLITE_OK);
  DBG1("Status [%lli] has been stored in table ["%s"], SQLite database ["%s"].",
       static_cast<sqlite3_int64>(i_status),
       this->m_table_name.c_str(),
       this->m_db_name.c_str());

  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into table ["%s"] of database ["%s"] "
        "by statement ["%s"]!",
        this->m_table_name.c_str(),
        this->m_db_name.c_str(),
        insert_statement.c_str());
    this->__finalize_and_throw__(
        insert_statement.c_str(),
        SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG1("All insertions have succeeded.");
  }

#if ENABLED_DB_CACHING
  // TODO: caching the record
#endif

  this->__finalize__(insert_statement.c_str());
  this->__increment_rows__();
  Record record(
      record_id,
      i_balance,
      i_description,
      i_status,
      current_datetime);
  DBG1("Constructed output record.");

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit DailyTable::addRecord().");
  return (record);
}

Record DailyTable::readRecord(const ID_t& i_record_id) {
  INF("enter DailyTable::readRecord().");

#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_record_id);
#endif

  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE ID == '";
  select_statement += std::to_string(i_record_id);
  select_statement += "';";
  this->__prepare_statement__(select_statement);
  sqlite3_step(this->m_db_statement);
  ID_t id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG1("Read id [%lli] from  table ["%s"] of database ["%s"], "
       "input id was [%lli].",
       id, this->m_table_name.c_str(), this->m_db_name.c_str(), i_record_id);
  TABLE_ASSERT("Input record id does not equal to primary key value "
               "from database!" &&
               id == i_record_id);

  std::string date(reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 1)));
  std::string time(reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 2)));
  DateTime datetime(date, time);
  MoneyValue_t balance = sqlite3_column_int64(this->m_db_statement, 3);
  const void* raw_description = sqlite3_column_text(this->m_db_statement, 4);
  WrappedString description(raw_description);
  sqlite3_int64 raw_status = sqlite3_column_int64(this->m_db_statement, 5);
  RecordStatus status(raw_status);
  DBG1("Loaded column data: Date ["%s"]; Time ["%s"]; "
       "Balance [%lli]; Description ["%s"]; Status [%lli].",
       datetime.getDate().c_str(),
       datetime.getTime().c_str(),
       balance,
       description.c_str(),
       raw_status);
  Record record(id, balance, description, status, datetime);
  DBG1("Proper record instance has been constructed.");

#if ENABLED_DB_CACHING
  // TODO: caching the record
#endif

  this->__finalize__(select_statement.c_str());
  INF("exit DailyTable::readRecord().");
  return (record);
}

const std::unordered_map<ID_t, Record> DailyTable::listRecords() {
  INF("enter DailyTable::listRecords().");
  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "';";
  this->__prepare_statement__(select_statement);

  std::unordered_map<ID_t, Record> records;
  int result = sqlite3_step(this->m_db_statement);
  while (result == SQLITE_OK) {
    ID_t id = sqlite3_column_int64(this->m_db_statement, 0);
    DBG1("Read id [%lli] from  table ["%s"] of database ["%s"].",
         id, this->m_table_name.c_str(), this->m_db_name.c_str());
    std::string date(reinterpret_cast<const char*>(
        sqlite3_column_text(this->m_db_statement, 1)));
    std::string time(reinterpret_cast<const char*>(
        sqlite3_column_text(this->m_db_statement, 2)));
    DateTime datetime(date, time);
    MoneyValue_t balance = sqlite3_column_int64(this->m_db_statement, 3);
    const void* raw_description = sqlite3_column_text(this->m_db_statement, 4);
    WrappedString description(raw_description);
    sqlite3_int64 raw_status = sqlite3_column_int64(this->m_db_statement, 5);
    RecordStatus status(raw_status);
    DBG1("Loaded column data: Date ["%s"]; Time ["%s"]; "
         "Balance [%lli]; Description ["%s"]; Status [%lli].",
         datetime.getDate().c_str(),
         datetime.getTime().c_str(),
         balance,
         description.c_str(),
         raw_status);
    Record record(id, balance, description, status, datetime);
    DBG1("Proper record instance has been constructed.");
    records[id] = record;
    DBG1("Record has been inserted into map.");
    result = sqlite3_step(this->m_db_statement);
  }

#if ENABLED_DB_CACHING
  // TODO: caching the record
#endif

  this->__finalize__(select_statement.c_str());
  INF("exit DailyTable::listRecords().");
  return (records);
}

void DailyTable::deleteRecord(const ID_t& i_record_id) {
  INF("enter DailyTable::deleteRecord().");

#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_record_id);
#endif

  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID == '";
  delete_statement += std::to_string(i_record_id);
  delete_statement += "';";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  this->__decrement_rows__();
  if (i_record_id + 1 == this->m_next_id) {
    ID_t last_row_id = this->__read_last_id__(this->m_table_name);
    this->m_next_id = last_row_id + 1;
    DBG1("Deleted record with largest ID. Next ID value is set to [%lli].",
         this->m_next_id);
  }
  if (this->__empty__()) {
    DBG1("Table ["%s"] has become empty. Next ID value is set to zero.",
         this->m_table_name.c_str());
    this->m_next_id = 0;
  }
  DBG1("Deleted record [ID: %lli] in table ["%s"].",
       i_record_id, this->m_table_name.c_str());

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit DailyTable::deleteRecord().");
}

void DailyTable::deleteRecords(std::vector<ID_t>& i_record_ids) {
  INF("enter DailyTable::deleteRecords().");
  std::sort(i_record_ids.begin(), i_record_ids.end());

#if ENABLED_ADVANCED_DEBUG
  for (ID_t& record_id : i_record_ids) {
    this->__where_check__(record_id);
  }
#endif

  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID IN(";
  delete_statement += vectorToString(i_record_ids, ", ");
  delete_statement += ");";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  this->__decrease_rows__(static_cast<int>(i_record_ids.size()));
  DBG1("Updating next ID if necessary, initial value is [%lli].",
       this->m_next_id);
  for (std::vector<ID_t>::const_reverse_iterator it = i_record_ids.rbegin();
       it != i_record_ids.rend();
       ++it) {
    if (*it + 1 == this->m_next_id) {
      --this->m_next_id;
      DBG1("Deleted record with largest ID. "
           "Next ID value has been decremented.");
    } else {
      break;
    }
  }
  ID_t last_row_id = this->__read_last_id__(this->m_table_name);
  this->m_next_id = last_row_id + 1;
  DBG1("Deleted record with largest ID. Next ID value is set to [%lli].",
       this->m_next_id);
  DBG1("Finished updating next ID: [%lli].", this->m_next_id);
  if (this->__empty__()) {
    DBG1("Table ["%s"] has become empty. Next id value is set to zero.",
         this->m_table_name.c_str());
    this->m_next_id = 0;
  }
  DBG1("Deleted %lli records from table ["%s"].",
       static_cast<long long int>(i_record_ids.size()),
       this->m_table_name.c_str());

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit DailyTable::deleteRecords().");
}

const std::string& DailyTable::getName() const {
  INF("enter DailyTable::getName().");
  DBG1("Return the name ["%s"] of DailyTable at %p.",
       this->m_table_name.c_str(), this);
  INF("exit DailyTable::getName().");
  return (this->m_table_name);
}


/* Private members */
// ----------------------------------------------------------------------------
void DailyTable::__init__() {
  DBG2("enter DailyTable::__init__().");
  iDatabase::__init__();
  ID_t last_row_id = this->__read_last_id__(this->m_table_name);
  this->m_next_id = last_row_id == 0 ? 0 : last_row_id + 1;
  TRC("Initialization has completed: total rows [%i], "
      "last row id [%lli], next_id [%lli].",
      this->m_rows, last_row_id, this->m_next_id);
  DBG2("exit DailyTable::__init__().");
}

void DailyTable::__create_table__() {
  DBG2("enter DailyTable::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += this->m_table_name;
  statement += "('ID' INTEGER PRIMARY KEY, "
      "'Date' TEXT, "
      "'Time' TEXT, "
      "'Balance' INTEGER, "
      "'Description' TEXT, "
      "'Status' INTEGER);";
  this->__prepare_statement__(statement);
  sqlite3_step(this->m_db_statement);
  DBG2("Table ["%s"] has been successfully created.",
       this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG2("exit DailyTable::__create_table__().");
}

}  /* namespace mw */
