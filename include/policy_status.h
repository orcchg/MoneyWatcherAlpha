/*
 * policy_status.h
 *
 *  Description: Policy status class declaration.
 *
 *  Created on: 22.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef POLICY_STATUS_H_
#define POLICY_STATUS_H_

#include "types.h"


namespace mw {

enum PolicyStatusValue { PSV_UNKNOWN = -1,
                         PSV_ENABLED = 0,
                         PSV_DISABLED = 1,
                         PSV_APPLIED = 2,
                         PSV_PENDING = 3,
                         PSV_COUNT
};

/// @class PolicyStatus
/// @brief Represents various statuses of policy.
class PolicyStatus {
public:
  PolicyStatus(const sqlite3_int64& status);
  virtual ~PolicyStatus();

  const PolicyStatusValue& getStatus() const;
  void setStatus(const PolicyStatusValue& status);

  operator sqlite3_int64() const;
  bool operator == (const PolicyStatus& rhs) const;
  bool operator != (const PolicyStatus& rhs) const;

private:
  PolicyStatusValue m_status;
};

}  /* namespace mw */

#endif /* POLICY_STATUS_H_ */
