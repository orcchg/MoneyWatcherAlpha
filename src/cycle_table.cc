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
#include "service.h"


namespace mw {

int CycleTable::OPENED_CYCLE_TABLES_COUNT = 0;

CycleTable::CycleTable(const std::string& i_db_name)
  : iDatabase(i_db_name)
  , m_next_id(0)
  , m_table_name("Cycle_Table") {
  INF("enter CycleTable constructor.");
  this->__init__(this->m_table_name);
  ++CycleTable::OPENED_CYCLE_TABLES_COUNT;
  INF("exit CycleTable constructor.");
}

CycleTable::~CycleTable() {
  INF("enter CycleTable destructor.");
  this->__close_database__();
  --CycleTable::OPENED_CYCLE_TABLES_COUNT;
  INF("exit CycleTable destructor.");
}

Entry CycleTable::addEntry(
    const WrappedString& i_name,
    const WrappedString& i_description,
    const MoneyValue_t& i_current_balance) {
  INF("enter CycleTable::addEntry().");
  std::string insert_statement = "INSERT INTO \'";
  insert_statement += this->m_table_name;
  insert_statement += "\' VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8);";
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
  ID_t entry_id = this->m_next_id++;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, entry_id) == SQLITE_OK);
  DBG("ID [%lli] has been stored in SQLite database "%s".",
      entry_id, this->m_db_name.c_str());
  int name_n_bytes = i_name.n_bytes();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          2,
          i_name.c_str(),
          name_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Name ["%ls"] has been stored in SQLite database "%s".",
      i_name.c_str(), this->m_db_name.c_str());
  int description_n_bytes = i_description.n_bytes();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          3,
          i_description.c_str(),
          description_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Description ["%ls"] has been stored in SQLite database "%s".",
      i_description.c_str(), this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 4, i_current_balance) == SQLITE_OK);
  DBG("Current balance [%lli] has been stored in SQLite database "%s".",
      i_current_balance, this->m_db_name.c_str());
  MoneyValue_t last_transaction = 0;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 5, last_transaction) == SQLITE_OK);
  DBG("Last transaction balance [%lli] has been stored in SQLite database "%s".",
      last_transaction, this->m_db_name.c_str());
  DateTime current_datetime;
  std::string date = current_datetime.getDate();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          6,
          date.c_str(),
          date.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Date ["%s"] has been stored in SQLite database "%s".",
      date.c_str(), this->m_db_name.c_str());
  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          7,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Time ["%s"] has been stored in SQLite database "%s".",
      time.c_str(), this->m_db_name.c_str());
  Status status = SV_UNKNOWN;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 8, static_cast<sqlite3_int64>(status)) == SQLITE_OK);
  DBG("Status [%lli] has been stored in SQLite database "%s".",
      static_cast<sqlite3_int64>(status), this->m_db_name.c_str());
  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into database "%s" by statement "%s"!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(insert_statement.c_str());
  } else {
    DBG("All insertions have succeeded.");
  }
#if ENABLED_DB_CACHING
  // TODO: caching the entry
#endif
  this->__finalize__(insert_statement.c_str());
  Entry entry(
      entry_id,
      i_name,
      i_description,
      i_current_balance,
      last_transaction,
      status,
      current_datetime);
  DBG("Constructed output entry.");
  INF("exit CycleTable::addEntry().");
  return (entry);
}

Entry CycleTable::readEntry(const ID_t& i_entry_id) {
  INF("enter CycleTable::readEntry().");
  std::string select_statement = "SELECT * FROM \'";
  select_statement += this->m_table_name;
  select_statement += "\' WHERE ID == \'";
  select_statement += std::to_string(i_entry_id);
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
  DBG("Read id [%lli] from database, input id was [%lli].", id, i_entry_id);
  assert("Input entry id does not equal to primary key value from database!" &&
         id == i_entry_id);
  const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 1));
  WrappedString name(raw_name);
  const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 2));
  WrappedString description(raw_description);
  MoneyValue_t balance = sqlite3_column_int64(this->m_db_statement, 3);
  MoneyValue_t transaction = sqlite3_column_int64(this->m_db_statement, 4);
  std::string date(reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 5)));
  std::string time(reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 6)));
  DateTime datetime(date, time);
  sqlite3_int64 raw_status = sqlite3_column_int64(this->m_db_statement, 7);
  Status status(raw_status);
  DBG("Loaded column data: Name ["%ls"]; Description ["%ls"]; Balance [%lli]; Transaction [%lli]; Date ["%s"]; Time ["%s"]; Status [%lli].",
      name.c_str(), description.c_str(), balance, transaction, datetime.getDate().c_str(), datetime.getTime().c_str(), raw_status);
  Entry entry(id, name, description, balance, transaction, status, datetime);
  DBG("Proper entry instance has been constructed.");
#if ENABLED_DB_CACHING
  // TODO: caching the entry
#endif
  this->__finalize__(select_statement.c_str());
  INF("exit CycleTable::readEntry().");
  return (entry);
}

Entry CycleTable::updateEntry(
    const ID_t& i_entry_id,
    const MoneyValue_t& i_value,
    const WrappedString& i_description) {
  INF("enter CycleTable::updateEntry().");
  Entry entry = this->readEntry(i_entry_id);
  DBG("Got entry from SQLite database.");
  entry.updateBalance(i_value, i_description);
  DBG("Updated entry.");
  WrappedString update_statement = "UPDATE \'";
  update_statement += WrappedString(this->m_table_name);
  update_statement += "\' SET Description = \'";
  update_statement += i_description;
  update_statement += "\', CurrentBalance = \'";
  update_statement += WrappedString::to_string(entry.getBalance());
  update_statement += "\', LastTransaction = \'";
  update_statement += WrappedString::to_string(i_value);
  update_statement += "\', Date = \'";
  update_statement += WrappedString(entry.getDateTime().getDate());
  update_statement += "\', Time = \'";
  update_statement += WrappedString(entry.getDateTime().getTime());
  update_statement += "\', Status = \'";
  update_statement += WrappedString::to_string(static_cast<sqlite3_int64>(entry.getStatus()));
  update_statement += "\' WHERE ID == \'";
  update_statement += WrappedString::to_string(i_entry_id);
  update_statement += "\';";
  int nByte = update_statement.n_bytes();
  TRC("Provided string SQL statement: "%ls" of length %lli and bytes %i.",
      update_statement.c_str(), static_cast<long long int>(update_statement.length()), nByte);
  assert("Invalid database handler! Database probably was not open." &&
         this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      update_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  if (result != SQLITE_OK) {
    this->__finalize_and_throw__(update_statement.c_str());
  }
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
#if ENABLED_DB_CACHING
  // TODO: caching the entry
#endif
  this->__finalize__(update_statement.c_str());
  INF("exit CycleTable::updateEntry().");
  return (entry);
}


/* Private members */
// ----------------------------------------------------------------------------
void CycleTable::__init__(const std::string& i_table_name) {
  DBG("enter CycleTable::__init__().");
  iDatabase::__init__(i_table_name);
  DBG("exit CycleTable::__init__().");
}

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
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += i_table_name;
  statement += "('ID' INTEGER PRIMARY KEY, "
      "'Name' TEXT, "
      "'Description' TEXT, "
      "'CurrentBalance' INTEGER, "
      "'LastTransaction' INTEGER, "
      "'Date' TEXT, "
      "'Time' TEXT, "
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

void CycleTable::__finalize__(const wchar_t* i_statement) {
  DBG("enter CycleTable::__finalize__().");
  iDatabase::__finalize__(i_statement);
  DBG("exit CycleTable::__finalize__().");
}

void CycleTable::__finalize_and_throw__(const wchar_t* i_statement) {
  DBG("enter CycleTable::__finalize_and_throw__().");
  iDatabase::__finalize_and_throw__(i_statement);
}

}  /* namespace mw */
