/*
 * cycle_table.h
 *
 *  Description: Cycle Table class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef CYCLE_TABLE_H_
#define CYCLE_TABLE_H_

#include <string>
#include <unordered_map>
#include "common.h"
#include "entry.h"
#include "hash.h"
#include "idatabase.h"
#include "unistring.h"


namespace mw {

/// @class CycleTable
/// @brief Represents a table of entries, updating once per certain cycle.
class CycleTable : private iDatabase {
  template <typename Table>
  friend class TestAccessTable;
public:
  CycleTable(const std::string& db_name = "MW_CycleTable.db");
  virtual ~CycleTable();

  /// -------------------------------------------------------------------------
  /// @defgroup Entry Methods to produce or extract Entry instance.
  /// @{
  /// @brief Adds new entry to CycleTable.
  /// @param name - Name of entry.
  /// @param description - Text description of last transaction.
  /// @param current_balace - Value of current money balance of entry.
  /// @return Newly created entry.
  /// @details Entry will also be stored into SQLite database.
  Entry addEntry(
      const WrappedString& name,
      const WrappedString& description,
      const MoneyValue_t& current_balance);

  /// @brief Reads entry from SQLite database.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @return Newly read entry.
  Entry readEntry(const ID_t& entry_id);

  /// @brief Updates current balance of existing entry.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @param value - Amount to which the actual money balance should
  /// be updated - it could be both positive (income) or negative (expense)
  /// value.
  /// @param description - Description of provided transaction.
  /// @return Updated entry.
  Entry updateEntry(
      const ID_t& entry_id,
      const MoneyValue_t& value,
      const WrappedString& description);
  /// @}

  /// @brief Undoes last operation with existing entry, rolling its state
  /// back to last transaction.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @note If there is no last transaction provided with entry, the entry
  /// becomes empty.
  void undo(const ID_t& entry_id);
  /// -------------------------------------------------------------------------

  static int OPENED_CYCLE_TABLES_COUNT;

private:
  ID_t m_next_id;
  std::string m_table_name;
#if ENABLED_DB_CACHING
  __MW_DB_CACHED__ std::unordered_map<ID_t, Entry, Hasher<ID_t> > m_entries;
#endif

  void __init__(const std::string& table_name);
  void __open_database__();
  void __close_database__();
  void __create_table__(const std::string& table_name);
  bool __does_table_exist__(const std::string& table_name);
  int __count__(const std::string& table_name);
  bool __empty__(const std::string& table_name) const;
  void __increment_rows__();
  void __decrement_rows__();
  void __terminate__(const char* message);
  void __finalize__(const char* statement);
  void __finalize_and_throw__(const char* statement, int error_code);
  void __finalize__(const wchar_t* statement);
  void __finalize_and_throw__(const wchar_t* statement, int error_code);
  const char* __get_last_statement__() const;
  void __set_last_statement__(const char* statement);
};

}  /* namespace mw */

#endif /* CYCLE_TABLE_H_ */
