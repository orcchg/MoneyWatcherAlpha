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
class TableManager : public Singleton<TableManager> {
public:
  TableManager();
  virtual ~TableManager();

  /// -------------------------------------------------------------------------
  /// @defgroup GET Access methods for tables.
  /// @{
  CycleTable& getCycleTable();
  DailyTable& getDailyTable();
  /// @}
  /// -------------------------------------------------------------------------

  static int OPENED_DATABASES_COUNT;
  static const std::string single_database_name;

private:
  CycleTable m_cycle_table;
  DailyTable m_daily_table;
};

}  /* namespace mw */

#endif /* TABLE_MANAGER_H_ */
