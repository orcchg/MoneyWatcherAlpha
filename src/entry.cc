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
    const WrappedString& name,
    const WrappedString& description,
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

// ----------------------------------------------
ID_t Entry::getID() const {
  return (this->m_id);
}

const WrappedString& Entry::getName() const {
  return (this->m_name);
}

const WrappedString& Entry::getDescription() const {
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
Status Entry::updateBalance(
    const MoneyValue_t& i_value,
    const WrappedString& i_description) {
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
  this->m_status = status;
  TRC("Updated current balance of entry ["%s"] at ["%s" - "%s"] "
      "for value [%lli]. It was changed by [%lli].",
      this->m_name.c_str(),
      this->m_datetime.getDate().c_str(),
      this->m_datetime.getTime().c_str(),
      this->m_current_balance, i_value);
  DBG("exit Entry::updateBalance().");
  return (status);
}

Status Entry::rollbackBalance(
    const MoneyValue_t& i_value,
    const Record& i_record) {
  DBG("enter Entry::rollbackBalance().");
  this->m_description = i_record.getDescription();
  this->m_last_transaction = i_record.getBalance();
  this->m_current_balance -= i_value;
  this->m_datetime = i_record.getDateTime();
  this->m_status = i_record.getStatus();
  TRC("Rolled back current balance of entry ["%s"] at ["%s" - "%s"] "
      "for value [%lli]. It was changed by [%lli].",
      this->m_name.c_str(),
      this->m_datetime.getDate().c_str(),
      this->m_datetime.getTime().c_str(),
      this->m_current_balance, i_value);
  DBG("exit Entry::rollbackBalance().");
  return (this->m_status);
}

Record Entry::make_record() const {
  Record record(
      -1,  // proper ID will be assigned during database writing.
      this->m_last_transaction,
      this->m_description,
      this->m_status,
      this->m_datetime);
  return (record);
}

}  /* namespace mw */
