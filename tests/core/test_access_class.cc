/*
 * test_access_class.cc
 *
 *  Description: Test class to access private fields of core items, implementation.
 *
 *  Created on: 01.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "test_access_class.h"


namespace mw {

TestAccess::TestAccess(DailyTable* i_ptr_daily_table)
  : m_ptr_daily_table(i_ptr_daily_table)
  , m_next_id(DailyTable::next_id)
  , m_table_name(DailyTable::table_name) {
}

TestAccess::~TestAccess() {
}

const DB_Handler& TestAccess::getDbHandler() const {
  return (this->m_ptr_daily_table->m_db_handler);
}

ID_t TestAccess::getNextID() const {
  return (this->m_next_id);
}

const std::string& TestAccess::getTableName() const {
  return (this->m_table_name);
}

}  /* namespace mw */
