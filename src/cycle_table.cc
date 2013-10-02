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

CycleTable::CycleTable(const std::string& i_db_name)
  : iDatabase(i_db_name)
  , m_next_entry_id(0)
  , m_table_name("Cycle_Table") {
  DBG("enter CycleTable constructor.");
  this->__init__(this->m_table_name);
  ++CycleTable::OPENED_CYCLE_TABLES_COUNT;
  DBG("exit CycleTable constructor.");
}

CycleTable::~CycleTable() {
  DBG("enter CycleTable destructor.");
  this->__close_database__();
  --CycleTable::OPENED_CYCLE_TABLES_COUNT;
  DBG("exit CycleTable destructor.");
}

Entry CycleTable::addEntry(
    const std::wstring& i_name,
    const MoneyValue_t& i_current_balance) {
  DBG("enter CycleTable::addEntry().");
  std::string insert_statement = "INSERT INTO \'";
  insert_statement += this->m_table_name;
  insert_statement += "\' VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7);";
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
  ID_t entry_id = this->m_next_entry_id++;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, entry_id) == SQLITE_OK);
  DBG("ID [%lli] has been stored in SQLite database \"%s\".",
      entry_id, this->m_db_name.c_str());
  int name_n_bytes = i_name.length() * sizeof(wchar_t);
  accumulate = accumulate &&
      (sqlite3_bind_text16(
          this->m_db_statement,
          2,
          i_name.c_str(),
          name_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Name [\"%S\"] has been stored in SQLite database \"%s\".",
      i_name.c_str(), this->m_db_name.c_str());
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 3, i_current_balance) == SQLITE_OK);
  DBG("Current balance [%lli] has been stored in SQLite database \"%s\".",
      i_current_balance, this->m_db_name.c_str());
  MoneyValue_t last_transaction = 0;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 4, last_transaction) == SQLITE_OK);
  DBG("Last transaction balance [%lli] has been stored in SQLite database \"%s\".",
      last_transaction, this->m_db_name.c_str());
  DateTime current_datetime;
  std::string date = current_datetime.getDate();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          5,
          date.c_str(),
          date.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Date [\"%s\"] has been stored in SQLite database \"%s\".",
      date.c_str(), this->m_db_name.c_str());
  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          6,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Time [\"%s\"] has been stored in SQLite database \"%s\".",
      time.c_str(), this->m_db_name.c_str());
  Status status = SV_UNKNOWN;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 7, static_cast<sqlite3_int64>(status)) == SQLITE_OK);
  DBG("Status [%lli] has been stored in SQLite database \"%s\".",
      static_cast<sqlite3_int64>(status), this->m_db_name.c_str());
  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into database \"%s\" by statement \"%s\"!",
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
      i_current_balance,
      last_transaction,
      status,
      current_datetime);
  DBG("Constructed output entry.");
  DBG("exit CycleTable::addEntry().");
  return (entry);
}

Entry CycleTable::readEntry(const ID_t& entry_id) {
  //
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
      "'CurrentBalance' INTEGER, "
      "'LastTransaction' INTEGER, "
      "'Date' TEXT, "
      "'Time' TEXT, "
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
  DBG("Table \"%s\" has been successfully created.", i_table_name.c_str());
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

}  /* namespace mw */
