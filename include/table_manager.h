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

#include <string>
#include "cycle_table.h"
#include "daily_table.h"
#include "singleton.h"


namespace mw {

/// @class TableManager
/// @brief Contains tables and manages its content, applies policies.
class TableManager : public Singleton<TableManager>, private iDatabase {
  template <typename Table>
  friend class TestAccessTable;
public:
  TableManager();
  virtual ~TableManager();

  /// -------------------------------------------------------------------------
  /// @defgroup GET Access methods for tables.
  /// @{
  // TODO: CycleTable& getCycleTable();
  /// @}
  /// -------------------------------------------------------------------------

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
  /// @details Entry will also be updated in SQLite database.
  void update(
	    const ID_t& entry_id,
	    const MoneyValue_t& value,
	    const WrappedString& description);

  /// @brief
  void redo(const ID_t& entry_id);

  /// @brief Undoes last operation with existing entry, rolling its state
  /// back to last transaction.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @note If there is no last transaction provided with entry, the entry
  /// becomes empty. Also deletes associated record from daily_table.
  void undo(const ID_t& entry_id);
  /// @}
  /// -------------------------------------------------------------------------

  static int OPENED_DATABASES_COUNT;
  static const std::string single_database_name;

private:
  CycleTable m_cycle_table;
  DailyTable m_daily_table;
  std::string m_table_name;

  void __init__();
  void __create_table__();
};

}  /* namespace mw */

#endif /* TABLE_MANAGER_H_ */
