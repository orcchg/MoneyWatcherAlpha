/*
 * status.cc
 *
 *  Description:  Status class implementation.
 *
 *  Created on: 26.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "status.h"


namespace mw {

Status::Status(const sqlite3_int64& i_status)
  : m_status(SV_UNKNOWN) {
  int status = i_status % SV_COUNT;
  switch (status) {
    case SV_EXPENSE:
      m_status = SV_EXPENSE;
      break;
    case SV_INCOME:
      m_status = SV_INCOME;
      break;
    default:
      break;
  }
}

Status::~Status() {
}

const StatusValue& Status::getStatus() const {
  return (this->m_status);
}

void Status::setStatus(const StatusValue& status) {
  this->m_status = status;
}

Status::operator sqlite3_int64() const {
  return (static_cast<sqlite3_int64>(this->m_status));
}

bool Status::operator == (const Status& rhs) const {
  return (this->m_status == rhs.m_status);
}
bool Status::operator != (const Status& rhs) const {
  return (this->m_status != rhs.m_status);
}

}  /* namespace mw */
