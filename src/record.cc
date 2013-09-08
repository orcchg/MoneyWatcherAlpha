/*
 * record.cc
 *
 *  Description: Record class functionality implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <string>
#include "record.h"


namespace mw {

ID_t Record::getID() const {
  return (this->m_id);
}

const std::string& Record::getName() const {
  return (this->m_name);
}

const MoneyValue_t& Record::getBalance() const {
  return (this->m_balance);
}

const std::string& Record::getDescription() const {
  return (this->m_description);
}

const DateTime& Record::getDateTime() const {
  return (this->m_datetime);
}

const Status& Record::getStatus() const {
  return (this->m_status);
}

void* Record::getData() const {
  return (this->m_data);
}

}  /* namespace mw */
