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
#include <vector>
#include "common.h"
#include "hash.h"
#include "idatabase.h"
#include "record.h"
#include "test_access_class.h"
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
  DailyTable(DailyTable&& rval_obj);
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

  /// @brief Removes many records from SQLite database.
  /// @param record_ids - Vector of primary key of records of interest
  /// in SQLite database.
  /// @details Input array will be sorted.
  /// @note Value of next_id remains incremented in case any of record
  /// to be deleted is not the last one, otherwise next_id will be decreased.
  void deleteRecords(std::vector<ID_t>& record_ids);
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup GET Access methods of Daily Table.
  /// @{
  /// @brief Gets name of table in SQLite database.
  const std::string& getName() const;
  /// @}
  /// -------------------------------------------------------------------------

  static int OPENED_DAILY_TABLES_COUNT;

private:
#if ENABLED_DB_CACHING
  __MW_DB_CACHED__ std::unordered_map<ID_t, Record, Hasher<ID_t> > m_records;
#endif

  void __init__();
  void __create_table__();

  DailyTable(const DailyTable& obj) = delete;
  DailyTable& operator = (const DailyTable& rhs) = delete;
  DailyTable& operator = (DailyTable&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* DAILY_TABLE_H_ */
