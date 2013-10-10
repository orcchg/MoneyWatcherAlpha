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
#include <locale.h>
#include <unistd.h>
#include "tests/google/gtest/gtest.h"
#include "test_access_class.h"
#include "test_fixture.h"
#include "test_include_all.h"


TEST (SimpleDemoTest, /*DISABLED_*/SimpleDemo) {
  /*std::thread simple_test([&] () {
    EXPECT_EQ(1.0, 1.0);
  });*/
  EXPECT_EQ(1.0, 1.0);
}

/* CylceTable testing */
// ----------------------------------------------------------------------------
TEST (CycleTableTest, CreateCycleTable) {
  std::string test_cycle_table_db_filename = "Test-CycleTable.db";
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  try {
    mw::CycleTable cycle_table(test_cycle_table_db_filename);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::CycleTable> accessor(&cycle_table);
    EXPECT_TRUE(accessor.checkFinalized());
    int db_file_exists = access(test_cycle_table_db_filename.c_str(), F_OK);
    EXPECT_TRUE(db_file_exists == 0);
    std::string statement = "SELECT * FROM \'";
    statement += accessor.getTableName();
    statement += "\';";
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        statement.c_str(),
        statement.length(),
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = SQLITE_ERROR;
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  remove(test_cycle_table_db_filename.c_str());
}

TEST (CycleTableTest, AddEntry) {
  std::string test_cycle_table_db_filename = "Test-CycleTable.db";
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  try {
    mw::CycleTable cycle_table(test_cycle_table_db_filename);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::CycleTable> accessor(&cycle_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя слота";
    mw::WrappedString s_description = "Тестовое описание слота";
    MoneyValue_t s_balance = 1000;
    MoneyValue_t s_transaction = 0;
    mw::Status s_status(mw::SV_UNKNOWN);
    mw::Entry entry = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(entry.getID(), accessor.getNextID() - 1);
    EXPECT_STREQ(entry.getName().c_str(), s_name.c_str());
    EXPECT_STREQ(entry.getDescription().c_str(), s_description.c_str());
    EXPECT_EQ(entry.getBalance(), s_balance);
    EXPECT_EQ(entry.getLastTransaction(), s_transaction);
    EXPECT_EQ(entry.getStatus(), s_status);

    std::string count_statement = "SELECT COUNT(*) FROM \'";
    count_statement += accessor.getTableName();
    count_statement += "\';";
    int nByte = static_cast<int>(count_statement.length());
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        count_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    int is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    EXPECT_EQ(is_row, SQLITE_ROW);
    is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(is_row, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    std::string value_statement = "SELECT * FROM \'";
    value_statement += accessor.getTableName();
    value_statement += "\';";
    nByte = static_cast<int>(value_statement.length());
    result = SQLITE_ERROR;
    result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        value_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    is_row = SQLITE_ERROR;
    is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    EXPECT_EQ(is_row, SQLITE_ROW);
    ID_t id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(entry.getID(), id);
    const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
    mw::WrappedString name(static_cast<const wchar_t*>(raw_name));
    EXPECT_STREQ(entry.getName().c_str(), name.c_str());
    const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
    mw::WrappedString description(static_cast<const wchar_t*>(raw_description));
    EXPECT_STREQ(entry.getDescription().c_str(), description.c_str());
    MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
    EXPECT_EQ(entry.getBalance(), balance);
    MoneyValue_t transaction = sqlite3_column_int64(statement_handler, 4);
    EXPECT_EQ(entry.getLastTransaction(), transaction);
    std::string date(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 5)));
    std::string time(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 6)));
    mw::DateTime datetime(date, time);
    EXPECT_STREQ(entry.getDateTime().getDate().c_str(), datetime.getDate().c_str());
    EXPECT_STREQ(entry.getDateTime().getTime().c_str(), datetime.getTime().c_str());
    sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 7);
    mw::Status status(raw_status);
    EXPECT_EQ(entry.getStatus(), status);
    is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(is_row, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  remove(test_cycle_table_db_filename.c_str());
}

TEST (CycleTableTest, ReadEntry) {
  std::string test_cycle_table_db_filename = "Test-CycleTable.db";
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  try {
    mw::CycleTable cycle_table(test_cycle_table_db_filename);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::CycleTable> accessor(&cycle_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя слота";
    mw::WrappedString s_description = "Тестовое описание слота";
    MoneyValue_t s_balance = 1000;
    MoneyValue_t s_transaction = 0;
    mw::Status s_status(mw::SV_UNKNOWN);
    mw::Entry entry = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(entry.getID(), accessor.getNextID() - 1);
    EXPECT_STREQ(entry.getName().c_str(), s_name.c_str());
    EXPECT_STREQ(entry.getDescription().c_str(), s_description.c_str());
    EXPECT_EQ(entry.getBalance(), s_balance);
    EXPECT_EQ(entry.getLastTransaction(), s_transaction);
    EXPECT_EQ(entry.getStatus(), s_status);

    mw::Entry read_entry = cycle_table.readEntry(entry.getID());
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(read_entry.getID(), entry.getID());
    EXPECT_STREQ(read_entry.getName().c_str(), entry.getName().c_str());
    EXPECT_STREQ(read_entry.getDescription().c_str(), entry.getDescription().c_str());
    EXPECT_EQ(read_entry.getBalance(), entry.getBalance());
    EXPECT_EQ(read_entry.getLastTransaction(), entry.getLastTransaction());
    EXPECT_STREQ(read_entry.getDateTime().getDate().c_str(), entry.getDateTime().getDate().c_str());
    EXPECT_STREQ(read_entry.getDateTime().getTime().c_str(), entry.getDateTime().getTime().c_str());
    EXPECT_EQ(read_entry.getStatus(), entry.getStatus());

    EXPECT_TRUE(accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  remove(test_cycle_table_db_filename.c_str());
}

TEST (CycleTableTest, UpdateEntry) {
  std::string test_cycle_table_db_filename = "Test-CycleTable.db";
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  try {
    mw::CycleTable cycle_table(test_cycle_table_db_filename);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::CycleTable> accessor(&cycle_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя слота";
    mw::WrappedString s_description = "Тестовое описание слота";
    MoneyValue_t s_balance = 1000;
    mw::Entry entry = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());

    MoneyValue_t s_expense = -700;
    mw::WrappedString s_transaction_comment = "Расход на 700 единиц";
    entry.updateBalance(s_expense, s_transaction_comment);
    EXPECT_EQ(entry.getBalance(), s_balance + s_expense);
    EXPECT_EQ(entry.getLastTransaction(), s_expense);
    mw::Status s_status(mw::SV_EXPENSE);
    EXPECT_EQ(entry.getStatus(), s_status);

    mw::Entry updated_entry =
        cycle_table.updateEntry(entry.getID(), s_expense, s_transaction_comment);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(updated_entry.getID(), entry.getID());
    EXPECT_STREQ(updated_entry.getName().c_str(), entry.getName().c_str());
    EXPECT_STREQ(updated_entry.getDescription().c_str(), entry.getDescription().c_str());
    EXPECT_EQ(updated_entry.getBalance(), entry.getBalance());
    EXPECT_EQ(updated_entry.getLastTransaction(), entry.getLastTransaction());
    EXPECT_EQ(updated_entry.getStatus(), entry.getStatus());

    mw::Entry read_entry = cycle_table.readEntry(entry.getID());
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(read_entry.getID(), entry.getID());
    EXPECT_STREQ(read_entry.getName().c_str(), entry.getName().c_str());
    EXPECT_STREQ(read_entry.getDescription().c_str(), updated_entry.getDescription().c_str());
    EXPECT_EQ(read_entry.getBalance(), entry.getBalance());
    EXPECT_EQ(read_entry.getLastTransaction(), entry.getLastTransaction());
    EXPECT_STREQ(read_entry.getDateTime().getDate().c_str(), updated_entry.getDateTime().getDate().c_str());
    EXPECT_STREQ(read_entry.getDateTime().getTime().c_str(), updated_entry.getDateTime().getTime().c_str());
    EXPECT_EQ(read_entry.getStatus(), entry.getStatus());

    EXPECT_TRUE(accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  remove(test_cycle_table_db_filename.c_str());
}


/* DailyTable testing */
// ----------------------------------------------------------------------------
TEST (DailyTableTest, CreateDailyTable) {
  std::string test_daily_table_db_filename = "Test-DailyTable.db";
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
    mw::DailyTable daily_table(test_daily_table_db_filename);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::DailyTable> accessor(&daily_table);
    EXPECT_TRUE(accessor.checkFinalized());
    int db_file_exists = access(test_daily_table_db_filename.c_str(), F_OK);
    EXPECT_TRUE(db_file_exists == 0);
    std::string statement = "SELECT * FROM \'";
    statement += accessor.getTableName();
    statement += "\';";
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        statement.c_str(),
        statement.length(),
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = SQLITE_ERROR;
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, AddRecord) {
  std::string test_daily_table_db_filename = "Test-DailyTable.db";
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
    mw::DailyTable daily_table(test_daily_table_db_filename);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::DailyTable> accessor(&daily_table);
    EXPECT_TRUE(accessor.checkFinalized());
    MoneyValue_t s_balance = 1000;
    mw::WrappedString s_description = "Тестовая запись в таблице";
    mw::Status s_status(mw::SV_INCOME);
    mw::Record record = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(record.getID(), accessor.getNextID() - 1);
    EXPECT_EQ(record.getBalance(), s_balance);
    EXPECT_STREQ(record.getDescription().c_str(), s_description.c_str());
    EXPECT_EQ(record.getStatus(), s_status);

    std::string count_statement = "SELECT COUNT(*) FROM \'";
    count_statement += accessor.getTableName();
    count_statement += "\';";
    int nByte = static_cast<int>(count_statement.length());
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        count_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    int is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    EXPECT_EQ(is_row, SQLITE_ROW);
    is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(is_row, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    std::string value_statement = "SELECT * FROM \'";
    value_statement += accessor.getTableName();
    value_statement += "\';";
    nByte = static_cast<int>(value_statement.length());
    result = SQLITE_ERROR;
    result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        value_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    is_row = SQLITE_ERROR;
    is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    EXPECT_EQ(is_row, SQLITE_ROW);
    ID_t id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(record.getID(), id);
    std::string date(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1)));
    std::string time(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2)));
    mw::DateTime datetime(date, time);
    EXPECT_STREQ(record.getDateTime().getDate().c_str(), datetime.getDate().c_str());
    EXPECT_STREQ(record.getDateTime().getTime().c_str(), datetime.getTime().c_str());
    MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
    EXPECT_EQ(record.getBalance(), balance);
    const void* raw_description = sqlite3_column_text(statement_handler, 4);
    mw::WrappedString description(static_cast<const wchar_t*>(raw_description));
    EXPECT_STREQ(record.getDescription().c_str(), description.c_str());
    sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 5);
    mw::Status status(raw_status);
    EXPECT_EQ(record.getStatus(), status);
    is_row = sqlite3_step(statement_handler);
    EXPECT_EQ(is_row, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, ReadRecord) {
  std::string test_daily_table_db_filename = "Test-DailyTable.db";
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
    mw::DailyTable daily_table(test_daily_table_db_filename);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::DailyTable> accessor(&daily_table);
    EXPECT_TRUE(accessor.checkFinalized());
    MoneyValue_t s_balance = 1000;
    mw::WrappedString s_description = "Тестовая запись в таблице";
    mw::Status s_status(mw::SV_INCOME);
    mw::Record record = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());

    mw::Record read_record = daily_table.readRecord(record.getID());
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(read_record.getID(), record.getID());
    EXPECT_STREQ(read_record.getDateTime().getDate().c_str(), record.getDateTime().getDate().c_str());
    EXPECT_STREQ(read_record.getDateTime().getTime().c_str(), record.getDateTime().getTime().c_str());
    EXPECT_EQ(read_record.getBalance(), record.getBalance());
    EXPECT_STREQ(read_record.getDescription().c_str(), record.getDescription().c_str());
    EXPECT_EQ(read_record.getStatus(), record.getStatus());

    EXPECT_TRUE(accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}


/* SQLite database testing */
// ----------------------------------------------------------------------------
TEST (SQLiteDatabaseTest, SingleTable) {
  std::string test_single_db_filename = "Test-SingleTable.db";
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
    mw::CycleTable cycle_table(test_single_db_filename);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    mw::DailyTable daily_table(test_single_db_filename);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::CycleTable> cycle_accessor(&cycle_table);
    EXPECT_TRUE(cycle_accessor.checkFinalized());
    mw::TestAccessTable<mw::DailyTable> daily_accessor(&daily_table);
    EXPECT_TRUE(daily_accessor.checkFinalized());
    EXPECT_NE(cycle_accessor.getDbHandler(), daily_accessor.getDbHandler());
    DB_Handler db_handler = cycle_accessor.getDbHandler();
    EXPECT_TRUE(db_handler);

    mw::WrappedString s_name = "Имя слота";
    mw::WrappedString s_entry_description = "Тестовое описание слота";
    mw::WrappedString s_record_description = "Тестовая запись в таблице";
    MoneyValue_t s_entry_balance = 1000;
    MoneyValue_t s_record_balance = 500;
    MoneyValue_t s_transaction = -700;
    mw::Status unknown_status(mw::SV_UNKNOWN);
    mw::Status s_entry_status(mw::SV_EXPENSE);
    mw::Status s_record_status(mw::SV_INCOME);

    mw::Entry entry = cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
    EXPECT_TRUE(cycle_accessor.checkFinalized());
    EXPECT_EQ(entry.getID(), cycle_accessor.getNextID() - 1);
    EXPECT_STREQ(entry.getName().c_str(), s_name.c_str());
    EXPECT_STREQ(entry.getDescription().c_str(), s_entry_description.c_str());
    EXPECT_EQ(entry.getBalance(), s_entry_balance);
    EXPECT_EQ(entry.getLastTransaction(), 0);
    EXPECT_EQ(entry.getStatus(), unknown_status);

    mw::Record record = daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
    EXPECT_TRUE(daily_accessor.checkFinalized());
    EXPECT_EQ(record.getID(), daily_accessor.getNextID() - 1);
    EXPECT_EQ(record.getBalance(), s_record_balance);
    EXPECT_STREQ(record.getDescription().c_str(), s_record_description.c_str());
    EXPECT_EQ(record.getStatus(), s_record_status);

    std::string count_statement = "SELECT count(*) FROM "
        "sqlite_master WHERE type == 'table' "
        "AND name != 'android_metadata' AND name != 'sqlite_sequence';";
    int nByte = static_cast<int>(count_statement.length());
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        db_handler,
        count_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    int tables_count = 0;
    do {
      result = sqlite3_step(statement_handler);
      ++tables_count;
    } while(result == SQLITE_ROW);
    EXPECT_EQ(result, SQLITE_DONE);
    result = SQLITE_ERROR;
    EXPECT_EQ(tables_count , 2);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(cycle_accessor.checkFinalized());
    EXPECT_TRUE(daily_accessor.checkFinalized());
  } catch(mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]!", e.what());
    EXPECT_TRUE(false);
    remove(test_single_db_filename.c_str());
  } catch(...) {
    ERR("Got exception!");
    EXPECT_TRUE(false);
    remove(test_single_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_single_db_filename.c_str());
}


/* Main */
// ----------------------------------------------------------------------------
int main(int argc, char **argv) {
  //setlocale(LC_CTYPE, "");
  ::testing::InitGoogleTest(&argc, argv);
  return (RUN_ALL_TESTS());
}
