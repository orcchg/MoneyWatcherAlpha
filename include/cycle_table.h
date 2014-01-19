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
#include <vector>
#include "common.h"
#include "entry.h"
#include "hash.h"
#include "idatabase.h"
#include "test_access_class.h"
#include "unistring.h"


namespace mw {

/// @class CycleTable
/// @brief Represents a table of entries.
class CycleTable : private iDatabase {
  template <typename Table>
  friend class TestAccessTable;
public:
  CycleTable(
      const std::string& db_name = "MW_CycleTable.db",
      const std::string& table_name = "Cycle_Table");
  CycleTable(CycleTable&& rval_obj);
  virtual ~CycleTable();

  /// -------------------------------------------------------------------------
  /// @defgroup Entry Methods to produce or extract Entry instance.
  /// @{
  /// @brief Adds new entry to CycleTable.
  /// @param name - Name of entry.
  /// @param description - Text description of last transaction.
  /// @param current_balace - Value of current money balance of entry.
  /// @return Newly created entry.Records
  /// @details Entry will also be stored into SQLite database.
  Entry addEntry(
      const WrappedString& name,
      const WrappedString& description,
      const MoneyValue_t& current_balance);

  /// @brief Reads entry from SQLite database.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @return Newly read entry.
  Entry readEntry(const ID_t& entry_id);

  /// @brief Reads all entries from CycleTable.
  /// @return Map with entries.
  const std::unordered_map<ID_t, Entry> listEntries();

  /// @brief Updates current balance and description of existing entry.
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

  /// @brief Same as above, but status of last transaction given.
  /// @brief status - Status of last transaction.
  Entry updateEntry(
      const ID_t& entry_id,
      const MoneyValue_t& value,
      const WrappedString& description,
      const RecordStatus& status);

  /// @brief Updates existing entry by values of given record,
  /// i.e. rolls entry back from its current state to those assigned
  /// in record.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @param value - Amount to which the actual money balance should
  /// be rolled back - it could be both positive (expense) or negative (income)
  /// value. Note the reversed logic compared to updateEntry().
  /// @param record - Record, whose balance, description and date & time
  /// values will be assigned to entry's data members.
  /// @return Rolled back entry.
  /// @details Entry will also be updated in SQLite database.
  Entry rollbackEntry(
      const ID_t& entry_id,
      const MoneyValue_t& value,
      const Record& record);

  /// @brief Removes entry from SQLite database.
  /// @param entry_id - Primary key of entry of interest in SQLite database.
  /// @note Value of next_id remains incremented in case the entry
  /// to be deleted is not the last one, otherwise next_id will be decreased.
  void deleteEntry(const ID_t& entry_id);

  /// @brief Removes many entries from SQLite database.
  /// @param entry_ids - Vector of primary key of entries of interest
  /// in SQLite database.
  /// @details Input array will be sorted.
  /// @note Value of next_id remains incremented in case any of entry
  /// to be deleted is not the last one, otherwise next_id will be decreased.
  void deleteEntries(std::vector<ID_t>& entry_ids);
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

private:
#if ENABLED_DB_CACHING
  __MW_DB_CACHED__ std::unordered_map<ID_t, Entry, Hasher<ID_t> > m_entries;
#endif

  void __init__();
  void __create_table__();

  /// @note Avoid code doubling.
  void __updateEntry__(const Entry& entry);

  CycleTable(const CycleTable& obj) = delete;
  CycleTable& operator = (const CycleTable& rhs) = delete;
  CycleTable& operator = (CycleTable&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* CYCLE_TABLE_H_ */
