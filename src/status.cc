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

RecordStatus::RecordStatus(const sqlite3_int64& i_status)
  : iStatus() {
  int status = i_status % RSV_COUNT;
  switch (status) {
    case RSV_EXPENSE:
      m_status = RSV_EXPENSE;
      break;
    case RSV_INCOME:
      m_status = RSV_INCOME;
      break;
    default:
      break;
  }
}


// ----------------------------------------------------------------------------
PolicyStatus::PolicyStatus(const sqlite3_int64& i_status)
  : iStatus() {
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

}  /* namespace mw */
