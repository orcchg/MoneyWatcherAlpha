/*
 * policy_table.h
 *
 *  Description: Table with records from applied policies, declaration.
 *
 *  Created on: 20.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef POLICY_TABLE_H_
#define POLICY_TABLE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include "common.h"
#include "policy.h"
#include "hash.h"
#include "idatabase.h"
#include "test_access_class.h"
#include "unistring.h"


namespace mw {

/// @class PolicyTable
/// @brief Represents a table of applied policies.
class PolicyTable : private iDatabase {
  template <typename Table>
  friend class TestAccessTable;
public:
  PolicyTable(const std::string& db_name = "MW_PolicyTable.db");
  PolicyTable(PolicyTable&& rval_obj);
  virtual ~PolicyTable();

  /// -------------------------------------------------------------------------
  /// @defgroup POLICY Methods to produce or extract Policy instance.
  /// @{
  /// @brief Adds new policy to PolicyTable.
  /// @param name - Name of policy.
  /// @param description - Text description of policy.
  /// @param ratio - Ratio, in which source entry's balance will be divided
  /// and added to the destination entry's balance.
  /// @param source_entry_id - ID of source entry.
  /// @param destination_entry_id - ID of destination entry.
  /// @param hours_period - Time period (in hours), policy will be
  /// applied at the end of it.
  /// @param status - Status of policy.
  /// @return Newly created record.
  /// @details Record will also be stored into SQLite database.
  Policy addPolicy(
      const WrappedString& name,
      const WrappedString& description,
      const PolicyRatio_t& ratio,
      const ID_t& source_entry_id,
      const ID_t& destination_entry_id,
      int hours_period,
      const PolicyStatus& status);

  /// @brief Reads policy from SQLite database.
  /// @param policy_id - Primary key of policy of interest in SQLite database.
  /// @return Newly read policy.
  Policy readPolicy(const ID_t& policy_id);
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup GET Access methods of Policy Table.
  /// @{
  /// @brief Gets name of table in SQLite database.
  const std::string& getName() const;
  /// @}
  /// -------------------------------------------------------------------------

  static int OPENED_POLICY_TABLES_COUNT;

private:
#if ENABLED_DB_CACHING
  __MW_DB_CACHED__ std::unordered_map<ID_t, Policy, Hasher<ID_t> > m_policies;
#endif

  void __init__();
  void __create_table__();

  PolicyTable(const PolicyTable& obj) = delete;
  PolicyTable& operator = (const PolicyTable& rhs) = delete;
  PolicyTable& operator = (PolicyTable&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* POLICY_TABLE_H_ */
