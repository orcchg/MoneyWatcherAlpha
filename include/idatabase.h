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


namespace mw {

class iDatabase {
public:
  iDatabase(const std::string& db_name = "Default-abstract.db");
  virtual ~iDatabase();

protected:
  std::string m_db_name;
  DB_Handler m_db_handler;
  DB_Statement m_db_statement;

  virtual void __open_database__() = 0;
  virtual void __close_database__() = 0;
  virtual void __create_table__(const std::string& table_name) = 0;
  virtual bool __does_table_exist__(const std::string& table_name) = 0;
  virtual void __terminate__(const char* message) = 0;
  virtual void __finalize__(const char* statement) = 0;
  virtual void __finalize_and_throw__(const char* statement) = 0;
};

/// @class TableException
/// @brief Represents a common exception raised by Table class methods.
class TableException : public std::exception {
public:
  TableException(const char* message);
  virtual ~TableException() throw();

  const char* what() const throw();

private:
  const char* m_message;
};

}  /* namespace mw */

#endif /* IDATABASE_H_ */
