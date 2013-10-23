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

RecordStatus::RecordStatus(RecordStatusValue i_status)
  : iStatus(i_status) {
}

RecordStatus::~RecordStatus() {
}

RecordStatus::RecordStatus(sqlite3_int64 i_status) {
  int status = i_status % 2;
  switch (status) {
    case 0:
      this->m_status = RSV_EXPENSE;
      break;
    case 1:
      this->m_status = RSV_INCOME;
      break;
    default:
      this->m_status = RSV_UNKNOWN;
      break;
  }
}


// ----------------------------------------------------------------------------
PolicyStatus::PolicyStatus(PolicyStatusValue i_status)
  : iStatus(i_status) {
}

PolicyStatus::PolicyStatus(sqlite3_int64 i_status) {
  int status = i_status % 4;
  switch (status) {
    case 0:
      this->m_status = PSV_ENABLED;
      break;
    case 1:
      this->m_status = PSV_DISABLED;
      break;
    case 2:
      this->m_status = PSV_APPLIED;
      break;
    case 3:
      this->m_status = PSV_PENDING;
      break;
    default:
      this->m_status = PSV_UNKNOWN;
      break;
  }
}

PolicyStatus::~PolicyStatus() {
}

}  /* namespace mw */
