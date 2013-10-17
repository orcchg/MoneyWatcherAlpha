/*
 * table_manager.h
 *
 *  Description: TableManager class declaration.
 *
 *  Created on: 12.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef TABLE_MANAGER_H_
#define TABLE_MANAGER_H_

#include <list>
#include <string>
#include <unordered_map>
#include "cycle_table.h"
#include "daily_table.h"
#include "hash.h"


namespace mw {

/// @class TableManager
/// @brief Contains tables and manages its content, applies policies.
class TableManager : private iDatabase {
  template <typename Table>
  friend class TestAccessTable;
  friend class TestAccessTableManager;
public:
  TableManager();
  TableManager(TableManager&& rval_obj);
  virtual ~TableManager();

  /// -------------------------------------------------------------------------
  /// @defgroup SERVICE Methods to manipulate with tables' content.
  /// @{
  /// @brief Adds new entry to CycleTable.
  /// @param name - Name of entry.
  /// @param description - Text description of last transaction.
  /// @param current_balace - Value of current money balance of entry.
  /// @return Newly created entry's ID.
  /// @details Entry will also be stored into SQLite database.
  ID_t add(
      const WrappedString& name,
      const WrappedString& description,
      const MoneyValue_t& current_balance);

  /// @brief Updates current balance of existing entry by its ID. Also,
  /// provides corresponding record in daily_table.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @param value - Amount to which the actual money balance should
  /// be updated - it could be both positive (income) or negative (expense)
  /// value.
  /// @param description - Description of provided transaction.
  /// @return ID of corresponding record.
  /// @details Entry will also be updated in SQLite database.
  ID_t update(
	    const ID_t& entry_id,
	    const MoneyValue_t& value,
	    const WrappedString& description);

  /// @brief Removes existing entry by its ID. Also removes all related records
  /// from daily_table through cascade deletion.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @note Result of this invocation cannot be undone.
  void remove(const ID_t& entry_id);

  /// @brief Undoes last operation with existing entry, rolling its state
  /// back to last transaction.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @note If there is no last transaction provided with entry, the entry
  /// becomes empty. Also deletes associated record from daily_table.
  void undo(const ID_t& entry_id);
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup NAME Getters for underlying tables' names.
  /// @{
  const std::string& getCycleTableName() const;
  const std::string& getDailyTableName() const;
  /// @}
  /// -------------------------------------------------------------------------

  static int OPENED_DATABASES_COUNT;
  static const std::string single_database_name;
  static const std::string records_table_name_prefix;

private:
  CycleTable m_cycle_table;
  DailyTable m_daily_table;
#if ENABLED_DB_CACHING
  __MW_DB_CACHED__ std::unordered_map<ID_t, std::list<ID_t>, Hasher<ID_t> > m_entry_records;
#endif

  void __init__();
  void __create_table__();
  void __create_table_entry_records__(const std::string& table_name);

  TableManager(const TableManager& obj) = delete;
  TableManager& operator = (const TableManager& rhs) = delete;
  TableManager& operator = (TableManager&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* TABLE_MANAGER_H_ */
