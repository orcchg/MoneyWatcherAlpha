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
public:
  TableManager();
  virtual ~TableManager();

  /// -------------------------------------------------------------------------
  /// @defgroup GET Access methods for tables.
  /// @{
  CycleTable& getCycleTable();
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup SERVICE Methods to manipulate with tables' content.
  /// @{
  /// @brief Updates current balance of existing entry by its ID. Also,
  /// provides corresponding record in daily_table.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @param value - Amount to which the actual money balance should
  /// be updated - it could be both positive (income) or negative (expense)
  /// value.
  /// @param description - Description of provided transaction.
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
  std::string m_entry_ids_table_name;

  void __init__(const std::string& table_name);
  void __create_table__(const std::string& table_name);
};

}  /* namespace mw */

#endif /* TABLE_MANAGER_H_ */
