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
#include "daily_table.h"
#include "types.h"


namespace mw {

class TestAccess {
public:
  TestAccess(DailyTable* ptr_daily_table);
  virtual ~TestAccess();

  const DB_Handler& getDbHandler() const;
  ID_t getNextID() const;
  const std::string& getTableName() const;

private:
  DailyTable* m_ptr_daily_table;
  ID_t m_next_id;
  std::string m_table_name;
};

}  /* namespace mw */

#endif /* TEST_ACCESS_CLASS_H_ */
