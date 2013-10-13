/*
 * table_manager.cc
 *
 *  Description: TableManager class implementation.
 *
 *  Created on: 12.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <string>
#include "logger.h"
#include "table_manager.h"


namespace mw {

int TableManager::OPENED_DATABASES_COUNT = 0;
const std::string TableManager::single_database_name = "MW-Database.db";

TableManager::TableManager()
  : iDatabase(TableManager::single_database_name)
  , m_cycle_table(TableManager::single_database_name)
  , m_daily_table(TableManager::single_database_name)
  , m_entry_ids_table_name("Entry_IDs_Table") {
  INF("enter TableManager constructor.");
  this->__init__(this->m_entry_ids_table_name);
  ++TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager constructor.");
}

TableManager::~TableManager() {
  INF("enter TableManager destructor.");
  this->__close_database__();
  --TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager destructor.");
}

/*CycleTable& TableManager::getCycleTable() {
  return (this->m_cycle_table);
}*/ // TODO:

ID_t TableManager::add(
    const WrappedString& i_name,
    const WrappedString& i_description,
    const MoneyValue_t& i_current_balance) {
  INF("enter TableManager::add().");
  Entry entry = this->m_cycle_table.addEntry(i_name, i_description, i_current_balance);
  ID_t entry_id = entry.getID();
  DBG("Added entry into Cycle_Table, got ID: %lli.", entry_id);
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_entry_ids_table_name;
  insert_statement += "' VALUES(?1);";
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
  int accumulate = sqlite3_bind_int64(this->m_db_statement, 1, entry_id);
  sqlite3_step(this->m_db_statement);
  if (accumulate != SQLITE_OK) {
    ERR("Error during saving data into database "%s" by statement "%s"!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(insert_statement.c_str(), SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG("All insertions have succeeded.");
  }
  this->__finalize__(insert_statement.c_str());
  INF("exit TableManager::add().");
  return (entry_id);
}

void TableManager::update(
      const ID_t& i_entry_id,
      const MoneyValue_t& i_value,
      const WrappedString& i_description){
  INF("enter TableManager::update().");
  Entry entry = this->m_cycle_table.updateEntry(i_entry_id, i_value, i_description);
  Record record = this->m_daily_table.addRecord(i_value, i_description, entry.getStatus());
  // TODO: create or update table for records, add foreign key.
  INF("exit TableManager::update().");
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
void TableManager::__init__(const std::string& i_table_name) {
  DBG("enter TableManager::__init__().");
  iDatabase::__init__(i_table_name);
  DBG("exit TableManager::__init__().");
}

void TableManager::__create_table__(const std::string& i_table_name) {
  DBG("enter TableManager::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += i_table_name;
  statement += "('EntryID' INTEGER PRIMARY KEY);";
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
  DBG("exit TableManager::__create_table__().");
}

}  /* namespace mw */
