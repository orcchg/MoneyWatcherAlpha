/*
 * test_all.cc
 *
 *  Description: Unit-tests for core testing.
 *
 *  Created on: 28.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <cstdio>
#include <thread>
#include <unistd.h>
#include "tests/google/gtest/gtest.h"
#include "test_fixture.h"
#include "test_include_all.h"


TEST (SimpleDemoTest, SimpleDemo) {
  /*std::thread simple_test([&] () {
    EXPECT_EQ(1.0, 1.0);
  });*/
  EXPECT_EQ(1.0, 1.0);
}

/* CylceTable testing */
// ----------------------------------------------------------------------------
TEST (CycleTableTest, CreateCycleTable) {
  EXPECT_TRUE(true);
}

/* DailyTable testing */
// ----------------------------------------------------------------------------
TEST (DailyTableTest, CreateDailyTable) {
  std::string test_daily_table_db_filename = "Test-DailyTable.db";
  mw::DailyTable daily_table(test_daily_table_db_filename);
  int db_file_exists = access(test_daily_table_db_filename.c_str(), F_OK);
  ASSERT_TRUE(db_file_exists == 0);
  //remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, AddRecord) {
  //
}

TEST (DailyTableTest, ReadRecord) {
  //
}

/* Main */
// ----------------------------------------------------------------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
