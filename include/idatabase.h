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
#include "common.h"
#include "types.h"

#define SQLITE_ACCUMULATED_PREPARE_ERROR -1
#define TABLE_ASSERTION_ERROR_CODE -2

#define EXPR_TO_STRING(x) #x
#define TABLE_ASSERT(expr)                                                     \
  ((expr)                                                                      \
   ? static_cast<void>(0)                                                      \
   : throw TableException(EXPR_TO_STRING(expr), TABLE_ASSERTION_ERROR_CODE))


namespace mw {

class iDatabase {
protected:
  iDatabase(
      const std::string& db_name = "Default-abstract.db",
      const std::string& table_name = "Default_Table");
  iDatabase(iDatabase&& rval_obj);
  virtual ~iDatabase();

protected:
  std::string m_db_name;
  std::string m_table_name;
  DB_Handler m_db_handler;
  DB_Statement m_db_statement;
  ID_t m_next_id;
  int m_rows;

  static const int sql_statement_limit_length = 1000000;  // million

  virtual void __init__() = 0;
  virtual void __create_table__() = 0;

  void __open_database__();
  void __close_database__();
  bool __does_table_exist__();
  int __count__(const std::string& i_table_name);
  bool __empty__() const;  // soft invocation
  void __increment_rows__();  // soft invocation
  void __increase_rows__(int value);  // soft invocation
  void __decrement_rows__();  // soft invocation
  void __decrease_rows__(int value);  // soft invocation
  void __terminate__(const char* message);
  void __finalize__(const char* statement);
  void __finalize_and_throw__(const char* statement, int error_code);
  const std::string& __get_table_name__() const;  // soft invocation
  const char* __get_last_statement__() const;  // soft invocation
  void __set_last_statement__(const char* statement);  // soft invocation
  void __create_table_for_last_id__(const std::string& table_name);
  void __write_last_id__(const std::string& table_name, const ID_t& last_id);
  ID_t __read_last_id__(const std::string& table_name);

#if ENABLED_ADVANCED_DEBUG
  void __where_check__(const ID_t& id);
  void __count_check__();
#endif

private:
  const char* m_last_statement;

  bool __check_rows_init__() const;

  iDatabase(const iDatabase& obj) = delete;
  iDatabase& operator = (const iDatabase& rhs) = delete;
  iDatabase& operator = (iDatabase&& rval_rhs) = delete;
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
