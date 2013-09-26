/*
 * daily_table.h
 *
 *  Description: Daily Table class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef DAILY_TABLE_H_
#define DAILY_TABLE_H_

#include <exception>
#include <string>
#include <unordered_map>
#include "hash.h"
#include "record.h"
#include "types.h"


namespace mw {

/// @class DailyTable
/// @brief Represents a table of daily changes.
class DailyTable {
public:
  DailyTable(const std::string& table_name = "Daily Table");
  virtual ~DailyTable();

  Record addRecord(
      const MoneyValue_t& balance,
      const std::wstring& description,
      const Status& status);

private:
  static ID_t next_id;

  std::string m_db_name;
  DB_Handler m_db_handler;
  DB_Statement m_db_statement;
  __MW_DB_CACHED__ std::unordered_map<ID_t, Record, Hasher<ID_t> > m_records;

  void __open_database__();
  void __close_database__();
  void __create_table__();
  void __terminate__(const char* message);
};

class DailyTableException : public std::exception {
public:
  DailyTableException(const char* message);
  virtual ~DailyTableException();

  const char* what() const throw();

private:
  const char* m_message;
};

}  /* namespace mw */

#endif /* DAILY_TABLE_H_ */
