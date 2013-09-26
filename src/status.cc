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

Status::Status(const Status_t& status)
  : m_status(status) {
}

Status::~Status() {
}

const Status_t& Status::getStatus() const {
  return (this->m_status);
}

void Status::setStatus(const Status_t& status) {
  this->m_status = status;
}

void Status::serialize(std::string* output) const {
  // TODO: serialize m_status to std::string
}

}  /* namespace mw */
