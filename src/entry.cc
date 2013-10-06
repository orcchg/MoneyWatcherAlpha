/*
 * entry.cc
 *
 *  Description: Entry class functionality implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <string>
#include "entry.h"


namespace mw {

Entry::Entry(
    const ID_t& id,
    const std::wstring& name,
    const std::wstring& description,
    const MoneyValue_t& current_balance,
    const MoneyValue_t& last_transaction,
    const Status& status,
    const DateTime& datetime)
    : m_id(id)
    , m_name(name)
    , m_description(description)
    , m_current_balance(current_balance)
    , m_last_transaction(last_transaction)
    , m_datetime(datetime)
    , m_status(status) {
}

Entry::~Entry() {
}

Record Entry::makeRecord() const {
  Record record(
      0,
      this->m_last_transaction,
      this->m_description,
      this->m_status,
      this->m_datetime);
  return (record);
}

ID_t Entry::getID() const {
  return (this->m_id);
}

const std::wstring& Entry::getName() const {
  return (this->m_name);
}

const std::wstring& Entry::getDescription() const {
  return (this->m_description);
}

const MoneyValue_t& Entry::getBalance() const {
  return (this->m_current_balance);
}

const MoneyValue_t& Entry::getLastTransaction() const {
  return (this->m_last_transaction);
}

const DateTime& Entry::getDateTime() const {
  return (this->m_datetime);
}

const Status& Entry::getStatus() const {
  return (this->m_status);
}

// ----------------------------------------------
void Entry::updateBalance(const MoneyValue_t& value) {
  // TODO(m.alov): Implement.
}

// ----------------------------------------------
void Entry::undo() {
  // TODO(m.alov): Implement.
}

void Entry::clear() {
  // TODO(m.alov): Implement.
}

}  /* namespace mw */
