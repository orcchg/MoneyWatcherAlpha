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

Record::Record(
    const std::wstring& name,
    const MoneyValue_t& balance,
    const std::wstring& description,
    const Status& status,
    void* data)
  : m_id(0)
  , m_name(name)
  , m_balance(balance)
  , m_description(description)
  , m_datetime(DateTime())
  , m_status(status)
  , m_data(data) {
}

Record::~Record() {
}

ID_t Record::getID() const {
  return (this->m_id);
}

const std::wstring& Record::getName() const {
  return (this->m_name);
}

const MoneyValue_t& Record::getBalance() const {
  return (this->m_balance);
}

const std::wstring& Record::getDescription() const {
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
