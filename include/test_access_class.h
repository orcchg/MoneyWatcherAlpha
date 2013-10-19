/*
 * test_access_class.h
 *
 *  Description: Test class to access private fields of core items, declaration.
 *
 *  Created on: 28.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef TEST_ACCESS_CLASS_H_
#define TEST_ACCESS_CLASS_H_

#include <string>
#include "logger.h"
#include "types.h"


namespace mw {

/// @class TestAccessTable
/// @brief Friend-class providing access to private Table class-members.
/// @note Template parameter 'Table' must inherits iDatabase interface.
template <typename Table>
class TestAccessTable {
public:
  TestAccessTable(Table* ptr_table);
  virtual ~TestAccessTable();

  const DB_Handler& getDbHandler() const;
  ID_t getNextID() const;
  const std::string& getTableName() const;
  int countRows(const std::string& table_name) const;

  bool checkFinalized() const;

private:
  Table* m_ptr_table;
};


template <typename Table>
TestAccessTable<Table>::TestAccessTable(Table* i_ptr_table)
  : m_ptr_table(i_ptr_table) {
}

template <typename Table>
TestAccessTable<Table>::~TestAccessTable() {
  m_ptr_table = nullptr;
}

template <typename Table>
const DB_Handler& TestAccessTable<Table>::getDbHandler() const {
  return (this->m_ptr_table->m_db_handler);
}

template <typename Table>
ID_t TestAccessTable<Table>::getNextID() const {
  return (this->m_ptr_table->m_next_id);
}

template <typename Table>
const std::string& TestAccessTable<Table>::getTableName() const {
  TRC("Table name: "%s".", this->m_ptr_table->m_table_name.c_str());
  return (this->m_ptr_table->m_table_name);
}

template <typename Table>
int TestAccessTable<Table>::countRows(const std::string& i_table_name) const {
  return (this->m_ptr_table->__count__(i_table_name));
}

template <typename Table>
bool TestAccessTable<Table>::checkFinalized() const {
  return (!this->m_ptr_table->m_db_statement);
}

}  /* namespace mw */

#endif /* TEST_ACCESS_CLASS_H_ */
