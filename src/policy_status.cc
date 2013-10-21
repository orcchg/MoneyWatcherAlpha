/*
 * policy_status.cc
 *
 *  Description: Policy status class implementation.
 *
 *  Created on: 22.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "policy_status.h"


namespace mw {

PolicyStatus::PolicyStatus(const sqlite3_int64& i_status)
  : m_status(PSV_UNKNOWN) {
  int status = i_status % PSV_COUNT;
  switch (status) {
    case PSV_ENABLED:
      m_status = PSV_ENABLED;
      break;
    case PSV_DISABLED:
      m_status = PSV_DISABLED;
      break;
    case PSV_APPLIED:
      m_status = PSV_APPLIED;
      break;
    case PSV_PENDING:
      m_status = PSV_PENDING;
      break;
    default:
      break;
  }
}

PolicyStatus::~PolicyStatus() {
}

const PolicyStatusValue& PolicyStatus::getStatus() const {
  return (this->m_status);
}

void PolicyStatus::setStatus(const PolicyStatusValue& status) {
  this->m_status = status;
}

PolicyStatus::operator sqlite3_int64() const {
  return (static_cast<sqlite3_int64>(this->m_status));
}

bool PolicyStatus::operator == (const PolicyStatus& rhs) const {
  return (this->m_status == rhs.m_status);
}
bool PolicyStatus::operator != (const PolicyStatus& rhs) const {
  return (this->m_status != rhs.m_status);
}

}  /* namespace mw */
