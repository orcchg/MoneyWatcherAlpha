/*
 * sqlite_driver.h
 *
 *  Description: Driver for SQLite 3 declaration.
 *
 *  Created on: 14.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef SQLITE_DRIVER_H_
#define SQLITE_DRIVER_H_

#include <string>
#include "sqlite3.h"


namespace db {

class SQLiteTable {
 public:
  SQLiteTable() {}
 private:
  std::string m_table_name;
};

}  /* namespace db */

#endif /* SQLITE_DRIVER_H_ */
