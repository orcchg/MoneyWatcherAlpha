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
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include "cycle_table.h"
#include "daily_table.h"
#include "hash.h"
#include "policy_table.h"


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
  /// @return Pair of newly created entry and corresponding record.
  /// @details Entry will also be stored into SQLite database. This function
  /// also provides record, storing entry's initial values, and it will also be
  /// stored into SQLite database.
  std::pair<Entry, Record> add(
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
  /// @return Correspondingly added record.
  /// @details Entry will also be updated in SQLite database.
  Record update(
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
  /// @return Rolled back entry.
  /// @note If there is no last transaction provided with entry, the entry
  /// becomes empty. Also deletes associated record from daily_table.
  Entry undo(const ID_t& entry_id);
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup POLICY Methods to provide policies for entries.
  /// @{
  /// @brief Creates policy linking two existing entries.
  /// @param name - Name of policy.
  /// @param description - Text description of policy.
  /// @param ratio - Ratio, in which source entry's balance will be divided
  /// and added to the destination entry's balance.
  /// @param source_entry_id - ID of source entry.
  /// @param destination_entry_id - ID of destination entry.
  /// @param hours_period - Time period (in hours), policy will be
  /// applied at the end of it.
  /// @param status - Status of policy.
  /// @return Newly created policy.
  /// @details Policy will also be stored into SQLite database.
  Policy createPolicy(
      const WrappedString& name,
      const WrappedString& description,
      const PolicyRatio_t& ratio,
      const ID_t& source_entry_id,
      const ID_t& destination_entry_id,
      int hours_period,
      const PolicyStatus& status);

  /// @brief Applies policy, determined by given ID.
  /// @param policy_id - Primary key of policy of interest in SQLite database.
  /// @return Record, corresponding to applied policy.
  /// @details Invocation of this function also makes record and stores it
  /// to daily_table in SQLite database.
  Record applyPolicy(const ID_t& policy_id);

  /// @brief Removes policy from SQLite database.
  /// @param policy_id - Primary key of policy of interest in SQLite database.
  /// @note Result of this invocation cannot be undone.
  void deletePolicy(const ID_t& policy_id);
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup NAME Getters for underlying tables' names.
  /// @{
  const std::string& getCycleTableName() const;
  const std::string& getDailyTableName() const;
  const std::string& getPolicyTableName() const;
  /// @}
  /// -------------------------------------------------------------------------

  static int OPENED_DATABASES_COUNT;
  static const std::string single_database_name;
  static const std::string records_table_name_prefix;

private:
  CycleTable m_cycle_table;
  DailyTable m_daily_table;
  PolicyTable m_policy_table;
  std::set<ID_t> m_entry_ids;
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
