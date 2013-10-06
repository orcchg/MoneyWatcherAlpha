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
#include "logger.h"


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
Status Entry::updateBalance(const MoneyValue_t& i_value, const std::wstring& i_description) {
  DBG("enter Entry::updateBalance().");
  Status status(SV_UNKNOWN);
  this->m_description = i_description;
  this->m_last_transaction = i_value;
  this->m_current_balance += i_value;  // update current balance
  this->m_datetime = DateTime();  // time of update
  if (i_value < 0) {
    status.setStatus(SV_EXPENSE);
    TRC("Balance update as expense.");
  } else {
    status.setStatus(SV_INCOME);
    TRC("Balance update as income.");
  }
  TRC("Updated current balance of entry \"%ls\" at \"%s\" \"%s\" for value %lli.",
      this->m_name.c_str(), this->m_datetime.getDate().c_str(), this->m_datetime.getTime().c_str(), i_value);
  DBG("exit Entry::updateBalance().");
  return (status);
}

// ----------------------------------------------
void Entry::undo() {
  // TODO(m.alov): Implement.
}

void Entry::clear() {
  // TODO(m.alov): Implement.
}


// ----------------------------------------------------------------------------
Record Entry::__make_record__() const {
  Record record(
      -1,  // proper ID will be assigned during database writing.
      this->m_last_transaction,
      this->m_description,
      this->m_status,
      this->m_datetime);
  return (record);
}

}  /* namespace mw */
