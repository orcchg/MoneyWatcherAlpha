/*
 * idatabase.cc
 *
 *  Description: Database protected interface implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <cassert>
#include <cstring>
#include <cwchar>
#include "common.h"
#include "idatabase.h"
#include "logger.h"

#define ROWS_IN_CASE_OF_NOT_EXISTING_TABLE -1


namespace mw {

iDatabase::iDatabase(const std::string& i_db_name)
  : m_db_name(i_db_name)
  , m_db_handler(nullptr)
  , m_db_statement(nullptr)
  , m_last_statement("")
  , m_rows(ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
}

iDatabase::~iDatabase() {
}

void iDatabase::__init__(const std::string& i_table_name) {
  this->__open_database__();
  try {
    this->__create_table__(i_table_name);
    this->m_rows = this->__count__(i_table_name);
  } catch(TableException& e) {
    ERR("%s", e.what());
    this->__terminate__("Error during create table or counting rows!");
    // Do not allow invalid object of DailyTable to be instantiated.
    throw e;
  }
}

void iDatabase::__open_database__() {
  int result = sqlite3_open(this->m_db_name.c_str(), &(this->m_db_handler));
  if (result != SQLITE_OK) {
    ERR("Unable to open database "%s"!", this->m_db_name.c_str());
    this->__terminate__("Error during open database.");
    throw TableException("Unable to open database!", result);
  }
  DBG("SQLite database "%s" has been successfully opened and placed into %p.",
      this->m_db_name.c_str(), this->m_db_handler);
}

void iDatabase::__close_database__() {
  if (this->m_db_statement) {
    DBG("Found prepared SQL statement at %p.", this->m_db_statement);
    this->__finalize__(this->m_last_statement);
  } else {
    DBG("Statement has been already finalized.");
  }
  if (this->m_db_handler) {
    DBG("Found valid database handler at %p.", this->m_db_handler);
    sqlite3_close(this->m_db_handler);
    this->m_db_handler = nullptr;
    DBG("Database "%s" has been successfully closed.",
        this->m_db_name.c_str());
  } else {
    DBG("Database "%s" has been already shut down.", this->m_db_name.c_str());
  }
  sqlite3_free(nullptr);
}

bool iDatabase::__does_table_exist__(const std::string& i_table_name) {
  std::string check_statement = "SELECT * FROM \'";
  check_statement += i_table_name;
  check_statement += "\';";
  int nByte = static_cast<int>(check_statement.length());
  TRC("Provided string SQL statement: "%s" of length %i.", check_statement.c_str(), nByte);
  TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
               this->m_db_handler);
  int result = sqlite3_prepare_v2(
      this->m_db_handler,
      check_statement.c_str(),
      nByte,
      &(this->m_db_statement),
      nullptr);
  this->__set_last_statement__(check_statement.c_str());
  sqlite3_step(this->m_db_statement);
  this->__finalize__(check_statement.c_str());
  bool table_exists = false;
  switch (result) {
    case SQLITE_OK:
      DBG("SQLite table "%s" already exists.", i_table_name.c_str());
      table_exists = true;
      break;
    default:
      DBG("SQLite table "%s" does not exist.", i_table_name.c_str());
      break;
  }
  DBG("exit iDatabase::__does_table_exist__().");
  return (table_exists);
}

int iDatabase::__count__(const std::string& table_name) {
  if (this->m_rows <= ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
    TRC("Rows count initialization has started.");
    std::string count_statement = "SELECT COUNT(*) FROM \'";
    count_statement += table_name;
    count_statement += "\';";
    int nByte = static_cast<int>(count_statement.length());
    TRC("Provided string SQL statement: "%s" of length %i.", count_statement.c_str(), nByte);
    TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
                 this->m_db_handler);
    int result = sqlite3_prepare_v2(
        this->m_db_handler,
        count_statement.c_str(),
        nByte,
        &(this->m_db_statement),
        nullptr);
    this->__set_last_statement__(count_statement.c_str());
    if (result != SQLITE_OK) {
      this->__finalize_and_throw__(count_statement.c_str(), result);
    }
    sqlite3_step(this->m_db_statement);
    this->m_rows = sqlite3_column_int(this->m_db_statement, 0);
    this->__finalize__(count_statement.c_str());
  }
  TRC("Number of rows: %i.", this->m_rows);
  DBG("exit iDatabase::__count__().");
  return (this->m_rows);
}

bool iDatabase::__empty__(const std::string& table_name) const {
  if (this->m_rows == ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
    ERR("Wrong initialization of database instance!");
    throw TableException(
        "Wrong initialization of database instance!",
        TABLE_ASSERTION_ERROR_CODE);
  }
  TRC("Number of rows: %i.", this->m_rows);
  DBG("exit iDatabase::__empty__().");
  return (this->m_rows == 0);
}

void iDatabase::__increment_rows__() {
  if (this->m_rows <= ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
    ERR("Wrong initialization of database instance!");
    throw TableException(
        "Wrong initialization of database instance!",
        TABLE_ASSERTION_ERROR_CODE);
  }
  ++this->m_rows;
}

void iDatabase::__decrement_rows__() {
  if (this->m_rows <= ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
    ERR("Wrong initialization of database instance!");
    throw TableException(
        "Wrong initialization of database instance!",
        TABLE_ASSERTION_ERROR_CODE);
  } else if (this->m_rows > 0) {
    --this->m_rows;
  }
}

void iDatabase::__terminate__(const char* i_message) {
  WRN("%s", i_message);
  sqlite3_close(this->m_db_handler);
  this->m_db_handler = nullptr;
  this->m_last_statement = "";
  TRC("Database "%s" has been shut down.", this->m_db_name.c_str());
  sqlite3_free(nullptr);
}

void iDatabase::__finalize__(const char* i_statement) {
  sqlite3_finalize(this->m_db_statement);
  this->m_db_statement = nullptr;
  this->m_last_statement = "";
  TRC("Statement "%s" (%i bytes) has been finalized.",
      i_statement, static_cast<int>(strlen(i_statement)) * sizeof(char));
}

void iDatabase::__finalize_and_throw__(const char* i_statement, int i_error_code) {
  ERR("Unable to prepare statement "%s" (%i bytes)!",
      i_statement, static_cast<int>(strlen(i_statement)) * sizeof(char));
  this->__finalize__(i_statement);
  DBG("exit iDatabase::__finalize_and_throw__().");
  throw TableException("Unable to prepare statement!", i_error_code);
}

void iDatabase::__finalize__(const wchar_t* i_statement) {
  sqlite3_finalize(this->m_db_statement);
  this->m_db_statement = nullptr;
  this->m_last_statement = "";
  TRC("Statement "%ls" (%i bytes) has been finalized.",
      i_statement, static_cast<int>(wcslen(i_statement)) * sizeof(wchar_t));
}

void iDatabase::__finalize_and_throw__(const wchar_t* i_statement, int i_error_code) {
  ERR("Unable to prepare statement "%ls" (%i bytes)!",
      i_statement, static_cast<int>(wcslen(i_statement)) * sizeof(wchar_t));
  this->__finalize__(i_statement);
  DBG("exit iDatabase::__finalize_and_throw__().");
  throw TableException("Unable to prepare statement!", i_error_code);
}

const char* iDatabase::__get_last_statement__() const {
  TRC("Got last recorded statement "%s".", this->m_last_statement);
  DBG("exit iDatabase::__get_last_statement__().");
  return (this->m_last_statement);
}

void iDatabase::__set_last_statement__(const char* i_statement) {
  TRC("Set new last statement "%s".", i_statement);
  this->m_last_statement = i_statement;
}


/* Table exception */
// ----------------------------------------------------------------------------
TableException::TableException(const char* i_message, int i_error_code)
  : m_message(i_message)
  , m_error_code(i_error_code) {
}

TableException::~TableException() throw() {
}

const char* TableException::what() const throw() {
  return (this->m_message);
}

int TableException::error() const throw() {
  return (this->m_error_code);
}

}  /* namespace mw */
