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

// ----------------------------------------------
ID_t Entry::getID() const {
  return (this->m_id);
}

const std::string& Entry::getName() const {
  return (this->m_name);
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

void* Entry::getData() const {
  return (this->m_data);
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
