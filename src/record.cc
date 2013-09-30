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
	const ID_t& id,
    const MoneyValue_t& balance,
    const std::wstring& description,
    const Status& status,
    const DateTime& datetime)
  : m_id(id)
  , m_balance(balance)
  , m_description(description)
  , m_datetime(datetime)
  , m_status(status) {
}

Record::~Record() {
}

ID_t Record::getID() const {
  return (this->m_id);
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

}  /* namespace mw */
