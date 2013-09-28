/*
 * test_all.cc
 *
 *  Description: Unit-tests for core testing.
 *
 *  Created on: 28.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <cstdio>
#include <string>
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
  // TODO: SQL-query to check if table exists
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, AddRecord) {
  mw::DailyTable daily_table("Test-DailyTable.db");
  MoneyValue_t balance = 1000;
  std::wstring description = L"Тестовая запись в таблице";
  mw::Status status(mw::SV_INCOME);
  mw::Record record = daily_table.addRecord(balance, description, status);
  EXPECT_EQ(record.getBalance(), balance);
  EXPECT_STREQ(record.getDescription().c_str(), description.c_str());
  EXPECT_EQ(record.getStatus(), status);
  // TODO: SQL-query to count rows in table
  // TODO: SQL-query to get column values and compare with actual in memory
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
