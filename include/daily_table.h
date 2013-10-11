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
#include "idatabase.h"
#include "record.h"
#include "types.h"
#include "unistring.h"


namespace mw {

/// @class DailyTable
/// @brief Represents a table of daily changes.
class DailyTable : private iDatabase {
  template <typename Table>
  friend class TestAccessTable;
public:
  DailyTable(const std::string& db_name = "MW_DailyTable.db");
  virtual ~DailyTable();

  /// -------------------------------------------------------------------------
  /// @defgroup RECORD Methods to produce or extract Record instance.
  /// @{
  /// @brief Adds new record to DailyTable.
  /// @param balance - Value of money balance for record.
  /// @param description - Text description of record.
  /// @param status - Status of record.
  /// @return Newly created record.
  /// @details Record will also be stored into SQLite database.
  Record addRecord(
      const MoneyValue_t& balance,
      const WrappedString& description,
      const Status& status);

  /// @brief Reads record from SQLite database.
  /// @param record_id - Primary key of record of interest in SQLite database.
  /// @return Newly read record.
  Record readRecord(const ID_t& record_id);
  /// @}

  /// @brief Removes record from SQLite database.
  /// @param record_id - Primary key of record of interest in SQLite database.
  /// @note Value of next_id remains incremented in case the record
  /// to be deleted is not the last one, otherwise next_id will be decreased.
  void deleteRecord(const ID_t& record_id);
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup GET Access methods of Daily Table.
  /// @{
  const std::string& getName() const;
  /// @}
  /// -------------------------------------------------------------------------

  /// @brief Loads existing DailyTable from SQLite database.
  /// @return True, if loading has succeeded, False otherwise.
  bool load();

  static int OPENED_DAILY_TABLES_COUNT;

private:
  ID_t m_next_id;
  std::string m_table_name;
#if ENABLED_DB_CACHING
  __MW_DB_CACHED__ std::unordered_map<ID_t, Record, Hasher<ID_t> > m_records;
#endif

  void __init__(const std::string& table_name);
  void __open_database__();
  void __close_database__();
  void __create_table__(const std::string& table_name);
  bool __does_table_exist__(const std::string& table_name);
  void __terminate__(const char* message);
  void __finalize__(const char* statement);
  void __finalize_and_throw__(const char* statement, int error_code);
  void __finalize__(const wchar_t* statement);
  void __finalize_and_throw__(const wchar_t* statement, int error_code);
  const char* __get_last_statement__() const;
  void __set_last_statement__(const char* statement);
};

}  /* namespace mw */

#endif /* DAILY_TABLE_H_ */
