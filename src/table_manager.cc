/*
 * table_manager.cc
 *
 *  Description: TableManager class implementation.
 *
 *  Created on: 12.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <string>
#include <utility>
#include "logger.h"
#include "table_manager.h"


namespace mw {

int TableManager::OPENED_DATABASES_COUNT = 0;
const std::string TableManager::single_database_name = "MW-Database.db";
const std::string TableManager::records_table_name_prefix = "Records_for_Entry_";

TableManager::TableManager()
  : iDatabase(TableManager::single_database_name, "Entry_IDs_Table")
  , m_cycle_table(TableManager::single_database_name)
  , m_daily_table(TableManager::single_database_name) {
  INF("enter TableManager constructor.");
  this->__init__();
  ++TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager constructor.");
}

TableManager::TableManager(TableManager&& rval_obj)
  : iDatabase(std::move(static_cast<iDatabase&>(rval_obj))) {
}

TableManager::~TableManager() {
  INF("enter TableManager destructor.");
  this->__close_database__();
  --TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager destructor.");
}

ID_t TableManager::add(
    const WrappedString& i_name,
    const WrappedString& i_description,
    const MoneyValue_t& i_current_balance) {
  INF("enter TableManager::add().");
  Entry entry = this->m_cycle_table.addEntry(i_name, i_description, i_current_balance);
  ID_t entry_id = entry.getID();
  DBG("Added entry into Cycle_Table, got ID: [%lli].", entry_id);
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_table_name;
  insert_statement += "' VALUES(?1, ?2);";
  int nByte = static_cast<int>(insert_statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.", insert_statement.c_str(), nByte);
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
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, entry_id) == SQLITE_OK);
  DBG1("ID [%lli] has been stored in SQLite database ["%s"].",
      entry_id, this->m_db_name.c_str());

  std::string records_table_name =
      TableManager::records_table_name_prefix + std::to_string(entry_id);
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          2,
          records_table_name.c_str(),
          records_table_name.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Records table name ["%s"] has been stored in SQLite database ["%s"].",
      records_table_name.c_str(), this->m_db_name.c_str());

  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into database ["%s"] by statement ["%s"]!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(insert_statement.c_str(), SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG2("All insertions have succeeded.");
  }

#if ENABLED_DB_CACHING
  // TODO: caching the entry's records
#endif

  this->__finalize__(insert_statement.c_str());
  this->__create_table_entry_records__(records_table_name);
  DBG1("Created table with name ["%s"] for records of entry [ID: %lli].",
      records_table_name.c_str(), entry_id);
  INF("exit TableManager::add().");
  return (entry_id);
}

ID_t TableManager::update(
    const ID_t& i_entry_id,
    const MoneyValue_t& i_value,
    const WrappedString& i_description){
  INF("enter TableManager::update().");
  Entry entry = this->m_cycle_table.updateEntry(i_entry_id, i_value, i_description);
  Record record = this->m_daily_table.addRecord(i_value, i_description, entry.getStatus());
  ID_t record_id = record.getID();

  DBG("Reading table name of entry records from SQLite database ["%s"]...", this->m_db_name.c_str());
  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE EntryID == '";
  select_statement += std::to_string(i_entry_id);
  select_statement += "';";
  int nByte = static_cast<int>(select_statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.", select_statement.c_str(), nByte);
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
  std::string records_table_name(reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 1)));
  DBG1("Got record [ID: %lli], inserting into table ["%s"] of entry [ID: %lli].",
      record_id, records_table_name.c_str(), entry.getID());
  this->__finalize__(select_statement.c_str());

  std::string insert_statement = "INSERT INTO '";
  insert_statement += records_table_name;
  insert_statement += "' VALUES(?1);";
  nByte = static_cast<int>(insert_statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.", insert_statement.c_str(), nByte);
  TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
               this->m_db_handler);
  result = sqlite3_prepare_v2(
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
  int accumulate = sqlite3_bind_int64(this->m_db_statement, 1, record_id);
  sqlite3_step(this->m_db_statement);
  if (accumulate != SQLITE_OK) {
    ERR("Error during saving data into database ["%s"] by statement ["%s"]!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(insert_statement.c_str(), SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG2("All insertions have succeeded.");
  }
  this->__finalize__(insert_statement.c_str());
  INF("exit TableManager::update().");
  return (record_id);
}

void TableManager::redo(const ID_t& entry_id) {
  INF("enter TableManager::redo().");
  // TODO: impl
  INF("exit TableManager::redo().");
}

void TableManager::undo(const ID_t& entry_id) {
  INF("enter TableManager::undo().");
  // TODO: impl
  INF("exit TableManager::undo().");
}


/* Private members */
// ----------------------------------------------------------------------------
void TableManager::__init__() {
  DBG("enter TableManager::__init__().");
  iDatabase::__init__();
  DBG("exit TableManager::__init__().");
}

void TableManager::__create_table__() {
  DBG("enter TableManager::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += this->m_table_name;
  statement += "('EntryID' INTEGER PRIMARY KEY, 'RecordsTableName' TEXT);";
  int nByte = static_cast<int>(statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.", statement.c_str(), nByte);
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
  DBG1("Table ["%s"] has been successfully created.", this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG("exit TableManager::__create_table__().");
}

void TableManager::__create_table_entry_records__(const std::string& i_table_name) {
  DBG("enter TableManager::__create_table_entry_records__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += i_table_name;
  statement += "('RecordID' INTEGER, FOREIGN KEY (RecordID) REFERENCES ";
  statement += this->m_table_name;
  statement += "(EntryID));";
  int nByte = static_cast<int>(statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.", statement.c_str(), nByte);
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
  DBG1("Table ["%s"] has been successfully created.", this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG("exit TableManager::__create_table_entry_records__().");
}

}  /* namespace mw */
