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
#define ID_IN_CASE_OF_NOT_EXISTING_TABLE 0


namespace mw {

iDatabase::iDatabase(
    const std::string& i_db_name,
    const std::string& i_table_name)
  : m_db_name(i_db_name)
  , m_table_name(i_table_name)
  , m_db_handler(nullptr)
  , m_db_statement(nullptr)
  , m_next_id(ID_IN_CASE_OF_NOT_EXISTING_TABLE)
  , m_rows(ROWS_IN_CASE_OF_NOT_EXISTING_TABLE)
  , m_last_statement("") {
}

iDatabase::iDatabase(iDatabase&& rval_obj)
  : m_db_name(rval_obj.m_db_name)
  , m_table_name(rval_obj.m_table_name)
  , m_db_handler(rval_obj.m_db_handler)
  , m_db_statement(rval_obj.m_db_statement)
  , m_next_id(rval_obj.m_next_id)
  , m_rows(rval_obj.m_rows)
  , m_last_statement(rval_obj.m_last_statement) {
  rval_obj.m_db_name = "";
  rval_obj.m_table_name = "";
  rval_obj.m_db_handler = nullptr;
  rval_obj.m_db_statement = nullptr;
  rval_obj.m_next_id = ID_IN_CASE_OF_NOT_EXISTING_TABLE;
  rval_obj.m_rows = ROWS_IN_CASE_OF_NOT_EXISTING_TABLE;
  rval_obj.m_last_statement = "";
}

iDatabase::~iDatabase() {
  this->m_db_name = "";
  this->m_table_name = "";
  this->m_db_handler = nullptr;
  this->m_db_statement = nullptr;
  this->m_next_id = ID_IN_CASE_OF_NOT_EXISTING_TABLE;
  this->m_rows = ROWS_IN_CASE_OF_NOT_EXISTING_TABLE;
  this->m_last_statement = "";
}

void iDatabase::__init__() {
  DBG("enter iDatabase::__init__().");
  this->__open_database__();
  try {
    this->__create_table__();
    this->m_rows = this->__count__(this->m_table_name);
  } catch(TableException& e) {
    ERR(["%s"], e.what());
    this->__terminate__("Error during create table or counting rows!");
    // Do not allow invalid object of DailyTable to be instantiated.
    WRN("throw from iDatabase::__init__().");
    throw e;
  }
  DBG("exit iDatabase::__init__().");
}

void iDatabase::__open_database__() {
  DBG("enter iDatabase::__open_database__().");
  int result = sqlite3_open(this->m_db_name.c_str(), &(this->m_db_handler));
  if (result != SQLITE_OK || !this->m_db_handler) {
    ERR("Unable to open database ["%s"]!",
        this->m_db_name.c_str());
    this->__terminate__("Error during open database.");
    WRN("throw from iDatabase::__open_database__().");
    throw TableException("Unable to open database!", result);
  }
  DBG1("SQLite database ["%s"] has been successfully opened and placed into %p.",
       this->m_db_name.c_str(), this->m_db_handler);
  DBG("exit iDatabase::__open_database__().");
}

void iDatabase::__close_database__() {
  DBG("enter iDatabase::__close_database__().");
  if (this->m_db_statement) {
    DBG1("Found prepared SQL statement at %p.",
         this->m_db_statement);
    this->__finalize__(this->m_last_statement);
  } else {
    DBG2("Statement has been already finalized.");
  }
  if (this->m_db_handler) {
    DBG("Found valid database handler at %p.",
        this->m_db_handler);
    sqlite3_close(this->m_db_handler);
    this->m_db_handler = nullptr;
    DBG("Database ["%s"] has been successfully closed.",
        this->m_db_name.c_str());
  } else {
    DBG1("Database ["%s"] has been already shut down.",
         this->m_db_name.c_str());
  }
  sqlite3_free(nullptr);
  DBG("exit iDatabase::__close_database__().");
}

bool iDatabase::__does_table_exist__() {
  DBG("enter iDatabase::__does_table_exist__().");
  std::string check_statement = "SELECT * FROM '";
  check_statement += this->m_table_name;
  check_statement += "';";
  int nByte = static_cast<int>(check_statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.",
      check_statement.c_str(), nByte);
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
      DBG("SQLite table ["%s"] already exists.",
          this->m_table_name.c_str());
      table_exists = true;
      break;
    default:
      DBG("SQLite table ["%s"] does not exist.",
          this->m_table_name.c_str());
      break;
  }
  DBG("exit iDatabase::__does_table_exist__().");
  return (table_exists);
}

int iDatabase::__count__(const std::string& i_table_name) {
  DBG("enter iDatabase::__count__().");
  if (this->m_rows <= ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
    TRC("Rows count initialization has started.");
    std::string count_statement = "SELECT COUNT(*) FROM '";
    count_statement += i_table_name;
    count_statement += "';";
    int nByte = static_cast<int>(count_statement.length());
    TRC("Provided string SQL statement: ["%s"] of length %i.",
        count_statement.c_str(), nByte);
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

bool iDatabase::__empty__() const {
  DBG("enter iDatabase::__empty__().");
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
  DBG("enter iDatabase::__increment_rows__().");
  if (this->m_rows <= ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
    ERR("Wrong initialization of database instance!");
    WRN("throw from iDatabase::__increment_rows__().");
    throw TableException(
        "Wrong initialization of database instance!",
        TABLE_ASSERTION_ERROR_CODE);
  }
  ++this->m_rows;
  DBG("exit iDatabase::__increment_rows__().");
}

void iDatabase::__decrement_rows__() {
  DBG("enter iDatabase::__decrement_rows__().");
  if (this->m_rows <= ROWS_IN_CASE_OF_NOT_EXISTING_TABLE) {
    ERR("Wrong initialization of database instance!");
    WRN("throw from iDatabase::__decrement_rows__().");
    throw TableException(
        "Wrong initialization of database instance!",
        TABLE_ASSERTION_ERROR_CODE);
  } else if (this->m_rows > 0) {
    --this->m_rows;
  }
  DBG("exit iDatabase::__decrement_rows__().");
}

void iDatabase::__terminate__(const char* i_message) {
  DBG("enter iDatabase::__terminate__().");
  WRN(["%s"], i_message);
  sqlite3_close(this->m_db_handler);
  this->m_db_handler = nullptr;
  this->m_last_statement = "";
  TRC("Database ["%s"] has been shut down.",
      this->m_db_name.c_str());
  sqlite3_free(nullptr);
  DBG("exit iDatabase::__terminate__().");
}

void iDatabase::__finalize__(const char* i_statement) {
  DBG("enter iDatabase::__finalize__().");
  sqlite3_finalize(this->m_db_statement);
  this->m_db_statement = nullptr;
  this->m_last_statement = "";
  TRC("Statement ["%s"] (%i bytes) has been finalized.",
      i_statement, static_cast<int>(strlen(i_statement) * sizeof(char)));
  DBG("exit iDatabase::__finalize__().");
}

void iDatabase::__finalize_and_throw__(const char* i_statement, int i_error_code) {
  DBG("enter iDatabase::__finalize_and_throw__().");
  ERR("Unable to prepare statement ["%s"] (%i bytes)!",
      i_statement, static_cast<int>(strlen(i_statement) * sizeof(char)));
  this->__finalize__(i_statement);
  DBG("exit iDatabase::__finalize_and_throw__().");
  throw TableException("Unable to prepare statement!", i_error_code);
}

const std::string& iDatabase::__get_table_name__() const {
  DBG("enter iDatabase::__get_table_name__().");
  TRC("Table name is ["%s"].",
      this->m_table_name.c_str());
  DBG("exit iDatabase::__get_table_name__().");
  return (this->m_table_name);
}

const char* iDatabase::__get_last_statement__() const {
  DBG("enter iDatabase::__get_last_statement__().");
  TRC("Got last recorded statement ["%s"].",
      this->m_last_statement);
  DBG("exit iDatabase::__get_last_statement__().");
  return (this->m_last_statement);
}

void iDatabase::__set_last_statement__(const char* i_statement) {
  DBG("enter iDatabase::__set_last_statement__().");
  TRC("Set new last statement ["%s"].",
      i_statement);
  this->m_last_statement = i_statement;
  DBG("exit iDatabase::__set_last_statement__().");
}

void iDatabase::__create_table_for_last_id__(const std::string& i_table_name) {
  DBG("enter iDatabase::__create_table_for_last_id__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += i_table_name;
  statement += "('LastRowID' INTEGER);";
  int nByte = static_cast<int>(statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.",
      statement.c_str(), nByte);
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
  DBG1("Table ["%s"] has been successfully created.",
       i_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG("exit iDatabase::__create_table_for_last_id__().");
}

void iDatabase::__write_last_id__(const std::string& i_table_name, const ID_t& i_last_id) {
  DBG("enter iDatabase::__write_last_id__().");
  std::string count_statement = "SELECT COUNT(*) FROM '";
  count_statement += i_table_name;
  count_statement += "';";
  int nByte = static_cast<int>(count_statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.",
      count_statement.c_str(), nByte);
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
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  int rows = sqlite3_column_int(this->m_db_statement, 0);
  this->__finalize__(count_statement.c_str());

  if (rows == 0) {
    DBG1("Table ["%s"] is empty. Inserting first value...",
         i_table_name.c_str());
    std::string insert_statement = "INSERT INTO '";
    insert_statement += i_table_name;
    insert_statement += "' VALUES(?1);";
    nByte = static_cast<int>(insert_statement.length());
    TRC("Provided string SQL statement: ["%s"] of length %i.",
        insert_statement.c_str(), nByte);
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
    int accumulate = sqlite3_bind_int64(this->m_db_statement, 1, i_last_id);
    sqlite3_step(this->m_db_statement);
    if (accumulate != SQLITE_OK) {
      ERR("Error during saving data into database ["%s"] by statement ["%s"]!",
          this->m_db_name.c_str(), insert_statement.c_str());
      this->__finalize_and_throw__(insert_statement.c_str(), SQLITE_ACCUMULATED_PREPARE_ERROR);
    } else {
      DBG2("All insertions have succeeded.");
    }
    this->__finalize__(insert_statement.c_str());

  } else {
    DBG("Table ["%s"] is filled. Updating first value...",
        i_table_name.c_str());
    std::string update_statement = "UPDATE '";
    update_statement += i_table_name;
    update_statement += "' SET LastRowID = '";
    update_statement += std::to_string(i_last_id);
    update_statement += "';";
    nByte = static_cast<int>(update_statement.length());
    TRC("Provided string SQL statement: ["%s"] of length %i.",
        update_statement.c_str(), nByte);
    TABLE_ASSERT("Invalid database handler! Database probably was not open." &&
                 this->m_db_handler);
    result = sqlite3_prepare_v2(
        this->m_db_handler,
        update_statement.c_str(),
        nByte,
        &(this->m_db_statement),
        nullptr);
    this->__set_last_statement__(update_statement.c_str());
    if (result != SQLITE_OK) {
      this->__finalize_and_throw__(update_statement.c_str(), result);
    }
    TRC("SQL statement has been compiled into byte-code and placed into %p.",
        this->m_db_statement);
    sqlite3_step(this->m_db_statement);
    this->__finalize__(update_statement.c_str());
  }
  DBG("exit iDatabase::__write_last_id__().");
}

ID_t iDatabase::__read_last_id__(const std::string& i_table_name) {
  DBG("enter iDatabase::__read_last_id__().");
  std::string count_statement = "SELECT * FROM '";
  count_statement += i_table_name;
  count_statement += "';";
  int nByte = static_cast<int>(count_statement.length());
  TRC("Provided string SQL statement: ["%s"] of length %i.",
      count_statement.c_str(), nByte);
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
  TRC("SQL statement has been compiled into byte-code and placed into %p.",
      this->m_db_statement);
  sqlite3_step(this->m_db_statement);
  ID_t last_row_id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG1("Read last row id: [%lli].",
       last_row_id);
  this->__finalize__(count_statement.c_str());
  DBG("exit iDatabase::__read_last_id__().");
  return (last_row_id);
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
