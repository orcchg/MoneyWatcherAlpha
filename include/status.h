/*
 * status.h
 *
 *  Description: Status class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef STATUS_H_
#define STATUS_H_

#include "types.h"


namespace mw {

template <typename StatusValue>
class iStatus {
public:
  iStatus();
  iStatus(StatusValue status);
  virtual ~iStatus();

  const StatusValue& getStatus() const;
  void setStatus(const StatusValue& status);

  operator sqlite3_int64() const;
  bool operator == (const iStatus& rhs) const;
  bool operator != (const iStatus& rhs) const;

protected:
  StatusValue m_status;
};


template <typename StatusValue>
iStatus<StatusValue>::iStatus()  {
}

template <typename StatusValue>
iStatus<StatusValue>::iStatus(StatusValue i_status)
  : m_status(i_status) {
}

template <typename StatusValue>
iStatus<StatusValue>::~iStatus() {
}

template <typename StatusValue>
const StatusValue& iStatus<StatusValue>::getStatus() const {
  return (this->m_status);
}

template <typename StatusValue>
void iStatus<StatusValue>::setStatus(const StatusValue& status) {
  this->m_status = status;
}

template <typename StatusValue>
iStatus<StatusValue>::operator sqlite3_int64() const {
  return (static_cast<sqlite3_int64>(this->m_status));
}

template <typename StatusValue>
bool iStatus<StatusValue>::operator == (const iStatus& rhs) const {
  return (this->m_status == rhs.m_status);
}

template <typename StatusValue>
bool iStatus<StatusValue>::operator != (const iStatus& rhs) const {
  return (this->m_status != rhs.m_status);
}


// ----------------------------------------------------------------------------
enum class RecordStatusValue : int {
  RSV_UNKNOWN = -1,
  RSV_EXPENSE = 0,
  RSV_INCOME = 1,
  RSV_APPLIED_POLICY = 2,
  RSV_COUNT
};

/// @class Status
/// @brief Represents various statuses of record.
class RecordStatus : public iStatus<RecordStatusValue> {
public:
  RecordStatus(RecordStatusValue status);
  RecordStatus(sqlite3_int64 status);
  virtual ~RecordStatus();
};


// ----------------------------------------------------------------------------
enum class PolicyStatusValue : int {
  PSV_UNKNOWN = -1,
  PSV_ENABLED = 0,
  PSV_DISABLED = 1,
  PSV_APPLIED = 2,
  PSV_PENDING = 3,
  PSV_COUNT
};

/// @class PolicyStatus
/// @brief Represents various statuses of policy.
class PolicyStatus : public iStatus<PolicyStatusValue> {
public:
  PolicyStatus(PolicyStatusValue status);
  PolicyStatus(sqlite3_int64 status);
  virtual ~PolicyStatus();
};

}  /* namespace mw */

#endif /* STATUS_H_ */
