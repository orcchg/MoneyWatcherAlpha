/*
 * test_fixture.h
 *
 *  Description: Test framework fixture declaration.
 *
 *  Created on: 28.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef TEST_FIXTURE_H_
#define TEST_FIXTURE_H_

#include <string>
#include "tests/google/gtest/gtest.h"
#include "daily_table.h"


namespace mw {

class DailyTableFixture : public ::testing::Test {
public:
  DailyTableFixture()
    : m_daily_table_db_name("Test-DailyTable.db")
    , m_daily_table(DailyTable(m_daily_table_db_name)) {
      // initialization code here
  }

  ~DailyTableFixture() {
      // cleanup any pending stuff, but no exceptions allowed
  }

  void SetUp() {
      // code here will execute just before the test ensues
  }

  void TearDown() {
      // code here will be called just after the test completes
      // ok to through exceptions from here if need be
  }

  const char* getDailyTableName() const {
    return (this->m_daily_table_db_name.c_str());
  }

  // put in any custom data members that you need
private:
  std::string m_daily_table_db_name;
  DailyTable m_daily_table;
};

}  /* namespace mw */

#endif /* TEST_FIXTURE_H_ */
