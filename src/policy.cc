/*
 * policy.cc
 *
 *  Description: Policy between two entries class implementation.
 *
 *  Created on: 20.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "policy.h"


namespace mw {

Policy::Policy(
    const ID_t& id,
    const WrappedString& name,
    const WrappedString& description,
    const PolicyRatio_t& ratio,
    const ID_t& source_entry_id,
    const ID_t& destination_entry_id,
    int hours_period,
    const PolicyStatus& status)
  : m_id(id)
  , m_name(name)
  , m_description(description)
  , m_ratio(ratio)
  , m_source_entry_id(source_entry_id)
  , m_destination_entry_id(destination_entry_id)
  , m_hours_period(hours_period)
  , m_status(status) {
}

Policy::~Policy() {
}

// ----------------------------------------------
ID_t Policy::getID() const {
  return (this->m_id);
}

const WrappedString& Policy::getName() const {
  return (this->m_name);
}

const WrappedString& Policy::getDescription() const {
  return (this->m_description);
}

const PolicyRatio_t& Policy::getRatio() const {
  return (this->m_ratio);
}

ID_t Policy::getSourceID() const {
  return (this->m_source_entry_id);
}

ID_t Policy::getDestinationID() const {
  return (this->m_destination_entry_id);
}

int Policy::getPeriod() const {
  return (this->m_hours_period);
}

const PolicyStatus& Policy::getStatus() const {
  return (this->m_status);
}

}  /* namespace mw */
