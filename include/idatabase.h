/*
 * idatabase.h
 *
 *  Description: Database protected interface declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef IDATABASE_H_
#define IDATABASE_H_

#include <string>
#include "types.h"

#define SQLITE_ACCUMULATED_PREPARE_ERROR -1
#define TABLE_ASSERTION_ERROR_CODE -2

#define EXPR_TO_STRING(x)	#x
#define TABLE_ASSERT(expr)                                                     \
  ((expr)                                                                      \
   ? static_cast<void>(0)                                                      \
   : throw TableException(EXPR_TO_STRING(expr), TABLE_ASSERTION_ERROR_CODE))


namespace mw {

class iDatabase {
public:
  iDatabase(const std::string& db_name = "Default-abstract.db");
  virtual ~iDatabase();

protected:
  std::string m_db_name;
  DB_Handler m_db_handler;
  DB_Statement m_db_statement;
  const char* m_last_statement;

  virtual void __init__(const std::string& i_table_name) = 0;
  virtual void __open_database__() = 0;
  virtual void __close_database__() = 0;
  virtual void __create_table__(const std::string& table_name) = 0;
  virtual bool __does_table_exist__(const std::string& table_name) = 0;
  virtual void __terminate__(const char* message) = 0;
  virtual void __finalize__(const char* statement) = 0;
  virtual void __finalize_and_throw__(const char* statement, int error_code) = 0;
  virtual void __finalize__(const wchar_t* statement) = 0;
  virtual void __finalize_and_throw__(const wchar_t* statement, int error_code) = 0;
  virtual const char* __get_last_statement__() const = 0;
  virtual void __set_last_statement__(const char* statement) = 0;
};

/// @class TableException
/// @brief Represents a common exception raised by Table class methods.
class TableException : public std::exception {
public:
  TableException(const char* message, int error_code);
  virtual ~TableException() throw();

  const char* what() const throw();
  int error() const throw();

private:
  const char* m_message;
  int m_error_code;
};

}  /* namespace mw */

#endif /* IDATABASE_H_ */
