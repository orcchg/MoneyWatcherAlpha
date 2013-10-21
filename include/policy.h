/*
 * policy.h
 *
 *  Description: Policy between two entries class declaration.
 *
 *  Created on: 20.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef POLICY_H_
#define POLICY_H_

#include "policy_status.h"
#include "types.h"
#include "unistring.h"


namespace mw {

class Policy {
public:
  Policy(
      const ID_t& id,
      const WrappedString& name,
      const WrappedString& description,
      const PolicyRatio_t& ratio,
      const ID_t& source_entry_id,
      const ID_t& destination_entry_id,
      int hours_period,
      const PolicyStatus& status);
  virtual ~Policy();

  /// -------------------------------------------------------------------------
  /// @defgroup GET Getters for class members.
  /// @{
  /// @brief Gets an ID of the current policy.
  ID_t getID() const;
  /// @brief Gets a name of the current policy.
  const WrappedString& getName() const;
  /// @brief Gets the descriptive comment supplied with the policy.
  const WrappedString& getDescription() const;
  /// @brief Gets ratio, which source entry's balance should be divided and
  /// than transmitted to the destination entry.
  const PolicyRatio_t& getRatio() const;
  /// @brief Gets source entry's ID.
  ID_t getSourceID() const;
  /// @brief Gets destination entry's ID.
  ID_t getDestinationID() const;
  /// @brief Gets duration of time period (in hours) current policy
  /// will be applied repeatedly.
  int getPeriod() const;
  /// @brief Gets the status of the last transaction.
  const PolicyStatus& getStatus() const;
  /// @}
  /// -------------------------------------------------------------------------

private:
  ID_t m_id;
  WrappedString m_name;
  WrappedString m_description;
  PolicyRatio_t m_ratio;
  ID_t m_source_entry_id;
  ID_t m_destination_entry_id;
  int m_hours_period;
  PolicyStatus m_status;
};

}  /* namespace mw */

#endif /* POLICY_H_ */
