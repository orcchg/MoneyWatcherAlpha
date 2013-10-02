/*
 * idatabase.cc
 *
 *  Description: Database protected interface implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <cassert>
#include "common.h"
#include "idatabase.h"
#include "logger.h"


namespace mw {

iDatabase::iDatabase(const std::string& i_db_name)
  : m_db_name(i_db_name)
  , m_db_handler(nullptr)
  , m_db_statement(nullptr) {
}

iDatabase::~iDatabase() {
}

void iDatabase::__open_database__() {
  int result = sqlite3_open(this->m_db_name.c_str(), &(this->m_db_handler));
  if (result != SQLITE_OK) {
    ERR("Unable to open database \"%s\"!", this->m_db_name.c_str());
    this->__terminate__("Error during open database.");
    throw TableException("Unable to open database!");
  }
  DBG("SQLite database \"%s\" has been successfully opened and placed into %p.",
      this->m_db_name.c_str(), this->m_db_handler);
}

void iDatabase::__close_database__() {
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
}

bool iDatabase::__does_table_exist__(const std::string& i_table_name) {
  std::string check_statement = "SELECT * FROM \'";
  check_statement += i_table_name;
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
      DBG("SQLite table \"%s\" already exists.", i_table_name.c_str());
      table_exists = true;
      break;
    default:
      DBG("SQLite table \"%s\" does not exist.", i_table_name.c_str());
      break;
  }
  DBG("exit iDatabase::__does_table_exist__().");
  return (table_exists);
}

void iDatabase::__terminate__(const char* message) {
  WRN(i_message);
  sqlite3_close(this->m_db_handler);
  this->m_db_handler = nullptr;
  TRC("Database \"%s\" has been shut down.", this->m_db_name.c_str());
  sqlite3_free(nullptr);
}

void iDatabase::__finalize__(const char* statement) {
  sqlite3_finalize(this->m_db_statement);
  this->m_db_statement = nullptr;
  TRC("Statement \"%s\" has been finalized.", statement);
}

void iDatabase::__finalize_and_throw__(const char* statement) {
  ERR("Unable to prepare statement \"%s\"!", statement);
  this->__finalize__(statement);
  DBG("exit iDatabase::__finalize_and_throw__().");
  throw TableException("Unable to prepare statement!");
}


/* Table exception */
// ----------------------------------------------------------------------------
TableException::TableException(const char* i_message)
  : m_message(i_message) {
}

TableException::~TableException() throw() {
}

const char* TableException::what() const throw() {
  return (this->m_message);
}

}  /* namespace mw */
