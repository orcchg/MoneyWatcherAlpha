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
  int status = i_status % 2;
  switch (status) {
    case 0:
      this->setStatus(RSV_EXPENSE);
      break;
    case 1:
      this->setStatus(RSV_INCOME);
      break;
    default:
      break;
  }
}

RecordStatus::~RecordStatus() {
}


// ----------------------------------------------------------------------------
PolicyStatus::PolicyStatus(const sqlite3_int64& i_status)
  : iStatus() {
  int status = i_status % 4;
  switch (status) {
    case 0:
      this->setStatus(PSV_ENABLED);
      break;
    case 1:
      this->setStatus(PSV_DISABLED);
      break;
    case 2:
      this->setStatus(PSV_APPLIED);
      break;
    case 3:
      this->setStatus(PSV_PENDING);
      break;
    default:
      break;
  }
}

PolicyStatus::~PolicyStatus() {
}

}  /* namespace mw */
