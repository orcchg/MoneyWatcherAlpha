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
#include "test_access_class.h"
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
  mw::TestAccess accessor(&daily_table);
  MoneyValue_t s_balance = 1000;
  std::wstring s_description = L"Тестовая запись в таблице";
  mw::Status s_status(mw::SV_INCOME);
  mw::Record record = daily_table.addRecord(s_balance, s_description, s_status);
  EXPECT_EQ(record.getID(), accessor.getNextID());  // TODO: check validity of next_id
  EXPECT_EQ(record.getBalance(), s_balance);
  EXPECT_STREQ(record.getDescription().c_str(), s_description.c_str());
  EXPECT_EQ(record.getStatus(), s_status);

  std::string count_statement = "SELECT COUNT(*) FROM \'";
  count_statement += accessor.getTableName();
  count_statement += "\'";
  int nByte = static_cast<int>(count_statement.length());
  DB_Statement statement_handler = nullptr;
  int result = sqlite3_prepare_v2(
      accessor.getDbHandler(),
      count_statement.c_str(),
      nByte,
      &statement_handler,
      nullptr);
  int is_row = sqlite3_step(statement_handler);
  ASSERT_EQ(result, SQLITE_OK);
  ASSERT_EQ(is_row, SQLITE_ROW);
  is_row = sqlite3_step(statement_handler);
  ASSERT_EQ(is_row, SQLITE_DONE);
  sqlite3_finalize(statement_handler);

  std::string value_statement = "SELECT * FROM \'";
  value_statement += accessor.getTableName();
  value_statement += "\'";
  nByte = static_cast<int>(value_statement.length());
  result = SQLITE_ERROR;
  result = sqlite3_prepare_v2(
      accessor.getDbHandler(),
      value_statement.c_str(),
      nByte,
      &statement_handler,
      nullptr);
  is_row = SQLITE_ERROR;
  is_row = sqlite3_step(statement_handler);
  ASSERT_EQ(result, SQLITE_OK);
  ASSERT_EQ(is_row, SQLITE_ROW);
  ID_t id = sqlite3_column_int64(statement_handler, 0);
  EXPECT_EQ(record.getID(), id);
  std::string date(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1)));
  std::string time(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2)));
  mw::DateTime datetime(date, time);
  EXPECT_EQ(record.getDateTime(), datetime);  // TODO: invalid datetime
  MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
  EXPECT_EQ(record.getBalance(), balance);
  const void* raw_description = sqlite3_column_text16(statement_handler, 4);
  std::wstring description(static_cast<const wchar_t*>(raw_description));
  EXPECT_STREQ(record.getDescription().c_str(), description.c_str());  // TODO: written only 1st word
  sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 5);
  mw::Status status(raw_status);
  EXPECT_EQ(record.getStatus(), status);
  is_row = sqlite3_step(statement_handler);
  ASSERT_EQ(is_row, SQLITE_DONE);
  sqlite3_finalize(statement_handler);
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
