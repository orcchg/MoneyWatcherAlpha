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
#include "tests/core/test_access_class.h"
#include "unistring.h"


namespace mw {

/// @class CycleTable
/// @brief Represents a table of entries, updating once per certain cycle.
class CycleTable : private iDatabase {
  template <typename Table>
  friend class TestAccessTable;
public:
  CycleTable(const std::string& db_name = "MW_CycleTable.db");
  CycleTable(CycleTable&& rval_obj);
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
  /// @details Entry will also be updated in SQLite database.
  Entry updateEntry(
      const ID_t& entry_id,
      const MoneyValue_t& value,
      const WrappedString& description);

  /// @brief Removes entry from SQLite database.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @note Value of next_id remains incremented in case the entry
  /// to be deleted is not the last one, otherwise next_id will be decreased.
  void deleteEntry(const ID_t& entry_id);
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup GET Access methods of Cycle Table.
  /// @{
  /// @brief Gets name of table in SQLite database.
  const std::string& getName() const;
  /// @}
  /// -------------------------------------------------------------------------

  static int OPENED_CYCLE_TABLES_COUNT;
  static const std::string last_row_id_table_name;

private:
#if ENABLED_DB_CACHING
  __MW_DB_CACHED__ std::unordered_map<ID_t, Entry, Hasher<ID_t> > m_entries;
#endif

  void __init__();
  void __create_table__();

  CycleTable(const CycleTable& obj) = delete;
  CycleTable& operator = (const CycleTable& rhs) = delete;
  CycleTable& operator = (CycleTable&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* CYCLE_TABLE_H_ */
