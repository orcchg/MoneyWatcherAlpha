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


static const char* intToSQLiteError(int i_error_code) {
  switch (i_error_code) {
  case SQLITE_OK:             /* 0  Successful result */
    return "SQLITE_OK";
  case SQLITE_ERROR:          /* 1  SQL error or missing database */
    return "SQLITE_ERROR";
  case SQLITE_INTERNAL:       /* 2  Internal logic error in SQLite */
    return "SQLITE_INTERNAL";
  case SQLITE_PERM:           /* 3  Access permission denied */
    return "SQLITE_PERM";
  case SQLITE_ABORT:          /* 4  Callback routine requested an abort */
    return "SQLITE_ABORT";
  case SQLITE_BUSY:           /* 5  The database file is locked */
    return "SQLITE_BUSY";
  case SQLITE_LOCKED:         /* 6  A table in the database is locked */
    return "SQLITE_LOCKED";
  case SQLITE_NOMEM:          /* 7  A malloc() failed */
    return "SQLITE_NOMEM";
  case SQLITE_READONLY:       /* 8  Attempt to write a readonly database */
    return "SQLITE_READONLY";
  case SQLITE_INTERRUPT:      /* 9  Operation terminated by sqlite3_interrupt()*/
    return "SQLITE_INTERRUPT";
  case SQLITE_IOERR:          /* 10  Some kind of disk I/O error occurred */
    return "SQLITE_IOERR";
  case SQLITE_CORRUPT:        /* 11  The database disk image is malformed */
    return "SQLITE_CORRUPT";
  case SQLITE_NOTFOUND:       /* 12  Unknown opcode in sqlite3_file_control() */
    return "SQLITE_NOTFOUND";
  case SQLITE_FULL:           /* 13  Insertion failed because database is full */
    return "SQLITE_FULL";
  case SQLITE_CANTOPEN:       /* 14  Unable to open the database file */
    return "SQLITE_CANTOPEN";
  case SQLITE_PROTOCOL:       /* 15  Database lock protocol error */
    return "SQLITE_PROTOCOL";
  case SQLITE_EMPTY:          /* 16  Database is empty */
    return "SQLITE_EMPTY";
  case SQLITE_SCHEMA:         /* 17  The database schema changed */
    return "SQLITE_SCHEMA";
  case SQLITE_TOOBIG:         /* 18  String or BLOB exceeds size limit */
    return "SQLITE_TOOBIG";
  case SQLITE_CONSTRAINT:     /* 19  Abort due to constraint violation */
    return "SQLITE_CONSTRAINT";
  case SQLITE_MISMATCH:       /* 20  Data type mismatch */
    return "SQLITE_MISMATCH";
  case SQLITE_MISUSE:         /* 21  Library used incorrectly */
    return "SQLITE_MISUSE";
  case SQLITE_NOLFS:          /* 22  Uses OS features not supported on host */
    return "SQLITE_NOLFS";
  case SQLITE_AUTH:           /* 23  Authorization denied */
    return "SQLITE_AUTH";
  case SQLITE_FORMAT:         /* 24  Auxiliary database format error */
    return "SQLITE_FORMAT";
  case SQLITE_RANGE:          /* 25  2nd parameter to sqlite3_bind out of range */
    return "SQLITE_RANGE";
  case SQLITE_NOTADB:         /* 26  File opened that is not a database file */
    return "SQLITE_NOTADB";
  case SQLITE_NOTICE:         /* 27  Notifications from sqlite3_log() */
    return "SQLITE_NOTICE";
  case SQLITE_WARNING:        /* 28  Warnings from sqlite3_log() */
    return "SQLITE_WARNING";
  case SQLITE_ROW:            /* 100  sqlite3_step() has another row ready */
    return "SQLITE_ROW";
  case SQLITE_DONE:           /* 101  sqlite3_step() has finished executing */
    return "SQLITE_DONE";
  default:
    return "UNKNOWN_ERROR_CODE";
  }
}

static int countRows(const std::string& table_name, const DB_Handler& handler) {
  std::string count_statement = "SELECT COUNT(*) FROM \'";
  count_statement += table_name;
  count_statement += "\';";
  int nByte = static_cast<int>(count_statement.length());
  DB_Statement statement_handler = nullptr;
  int result = sqlite3_prepare_v2(
      handler,
      count_statement.c_str(),
      nByte,
      &statement_handler,
      nullptr);
  EXPECT_TRUE(statement_handler);
  EXPECT_EQ(result, SQLITE_OK);
  result = sqlite3_step(statement_handler);
  EXPECT_EQ(result, SQLITE_ROW);
  EXPECT_EQ(sqlite3_column_count(statement_handler), 1);
  int type = sqlite3_column_type(statement_handler, 0);
  EXPECT_EQ(type, SQLITE_INTEGER);
  int value = sqlite3_column_int(statement_handler, 0);
  result = sqlite3_step(statement_handler);
  EXPECT_EQ(result, SQLITE_DONE);
  sqlite3_finalize(statement_handler);
  return value;
}


// ----------------------------------------------
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
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
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

    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);

    DB_Statement statement_handler = nullptr;
    std::string value_statement = "SELECT * FROM \'";
    value_statement += accessor.getTableName();
    value_statement += "\';";
    int nByte = static_cast<int>(value_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        value_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
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
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  remove(test_cycle_table_db_filename.c_str());
}

TEST (CycleTableTest, AddManyEntries) {
  std::string test_cycle_table_db_filename = "Test-CycleTable.db";
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  try {
    mw::CycleTable Cycle_table(test_cycle_table_db_filename);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::CycleTable> accessor(&Cycle_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя слота";
    mw::WrappedString s_description = "Тестовое описание слота";
    MoneyValue_t s_balance = 1000;
    mw::Status s_status(mw::SV_UNKNOWN);
    std::vector<mw::Entry> entries;
    entries.reserve(10);
    EXPECT_EQ(entries.size(), 0);
    mw::Entry entry_1 = Cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(entry_1.getID(), accessor.getNextID() - 1);
    entries.push_back(entry_1);
    mw::Entry entry_2 = Cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(entry_2.getID(), accessor.getNextID() - 1);
    entries.push_back(entry_2);
    mw::Entry entry_3 = Cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(entry_3.getID(), accessor.getNextID() - 1);
    entries.push_back(entry_3);
    mw::Entry entry_4 = Cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(entry_4.getID(), accessor.getNextID() - 1);
    entries.push_back(entry_4);
    mw::Entry entry_5 = Cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(entry_5.getID(), accessor.getNextID() - 1);
    entries.push_back(entry_5);
    EXPECT_EQ(accessor.getNextID(), 5);
    EXPECT_EQ(entries.size(), 5);

    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 5);

    DB_Statement statement_handler = nullptr;
    std::string value_statement = "SELECT * FROM \'";
    value_statement += accessor.getTableName();
    value_statement += "\';";
    int nByte = static_cast<int>(value_statement.length());
    int loop_result =
        sqlite3_prepare_v2(
            accessor.getDbHandler(),
            value_statement.c_str(),
            nByte,
            &statement_handler,
            nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(loop_result, SQLITE_OK);
    for (auto it = entries.begin(); it != entries.end(); ++it) {
      loop_result = sqlite3_step(statement_handler);
      EXPECT_EQ(loop_result, SQLITE_ROW);
      ID_t read_id = sqlite3_column_int64(statement_handler, 0);
      EXPECT_EQ(it->getID(), read_id);
      const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
      mw::WrappedString name(static_cast<const wchar_t*>(raw_name));
      EXPECT_STREQ(it->getName().c_str(), name.c_str());
      const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
      mw::WrappedString description(static_cast<const wchar_t*>(raw_description));
      EXPECT_STREQ(it->getDescription().c_str(), description.c_str());
      MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
      EXPECT_EQ(it->getBalance(), balance);
      MoneyValue_t transaction = sqlite3_column_int64(statement_handler, 4);
      EXPECT_EQ(it->getLastTransaction(), transaction);
      std::string date(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 5)));
      std::string time(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 6)));
      mw::DateTime datetime(date, time);
      EXPECT_STREQ(it->getDateTime().getDate().c_str(), datetime.getDate().c_str());
      EXPECT_STREQ(it->getDateTime().getTime().c_str(), datetime.getTime().c_str());
      sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 7);
      mw::Status status(raw_status);
      EXPECT_EQ(it->getStatus(), status);
    }
    loop_result = sqlite3_step(statement_handler);
    EXPECT_EQ(loop_result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
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
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  remove(test_cycle_table_db_filename.c_str());
}

TEST (CycleTableTest, ReadEntryWrongId) {
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

    int number_of_caught_exceptions = 0;
    try {
      mw::Entry read_entry = cycle_table.readEntry(entry.getID() + 5);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
    }
    EXPECT_EQ(number_of_caught_exceptions, 1);
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
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
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_cycle_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  remove(test_cycle_table_db_filename.c_str());
}

// TODO: Update invalid entry.


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
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
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

    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);

    DB_Statement statement_handler = nullptr;
    std::string value_statement = "SELECT * FROM \'";
    value_statement += accessor.getTableName();
    value_statement += "\';";
    int nByte = static_cast<int>(value_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        value_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
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
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, AddManyRecords) {
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
    std::vector<mw::Record> records;
    records.reserve(10);
    EXPECT_EQ(records.size(), 0);
    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(record_1.getID(), accessor.getNextID() - 1);
    records.push_back(record_1);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(record_2.getID(), accessor.getNextID() - 1);
    records.push_back(record_2);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(record_3.getID(), accessor.getNextID() - 1);
    records.push_back(record_3);
    mw::Record record_4 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(record_4.getID(), accessor.getNextID() - 1);
    records.push_back(record_4);
    mw::Record record_5 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(record_5.getID(), accessor.getNextID() - 1);
    records.push_back(record_5);
    EXPECT_EQ(accessor.getNextID(), 5);
    EXPECT_EQ(records.size(), 5);

    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 5);

    DB_Statement statement_handler = nullptr;
    std::string value_statement = "SELECT * FROM \'";
    value_statement += accessor.getTableName();
    value_statement += "\';";
    int nByte = static_cast<int>(value_statement.length());
    int loop_result =
        sqlite3_prepare_v2(
            accessor.getDbHandler(),
            value_statement.c_str(),
            nByte,
            &statement_handler,
            nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(loop_result, SQLITE_OK);
    for (auto it = records.begin(); it != records.end(); ++it) {
      loop_result = sqlite3_step(statement_handler);
      EXPECT_EQ(loop_result, SQLITE_ROW);
      ID_t read_id = sqlite3_column_int64(statement_handler, 0);
      EXPECT_EQ(it->getID(), read_id);
      std::string date(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1)));
      std::string time(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2)));
      mw::DateTime datetime(date, time);
      EXPECT_STREQ(it->getDateTime().getDate().c_str(), datetime.getDate().c_str());
      EXPECT_STREQ(it->getDateTime().getTime().c_str(), datetime.getTime().c_str());
      MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
      EXPECT_EQ(it->getBalance(), balance);
      const void* raw_description = sqlite3_column_text(statement_handler, 4);
      mw::WrappedString description(static_cast<const wchar_t*>(raw_description));
      EXPECT_STREQ(it->getDescription().c_str(), description.c_str());
      sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 5);
      mw::Status status(raw_status);
      EXPECT_EQ(it->getStatus(), status);
    }
    loop_result = sqlite3_step(statement_handler);
    EXPECT_EQ(loop_result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(accessor.checkFinalized());
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
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
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, ReadRecordWrongId) {
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

    int number_of_caught_exceptions = 0;
    try {
      mw::Record read_record = daily_table.readRecord(record.getID() + 5);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
    }
    EXPECT_EQ(number_of_caught_exceptions, 1);
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, DeleteRecord) {
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
    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_TRUE(accessor.checkFinalized());

    daily_table.deleteRecord(record_3.getID());
    EXPECT_EQ(accessor.getNextID(), 2);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 2);

    daily_table.deleteRecord(record_1.getID());
    EXPECT_EQ(accessor.getNextID(), 2);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);

    daily_table.deleteRecord(record_2.getID());
    EXPECT_EQ(accessor.getNextID(), 0);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);

  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, DeleteRecordWrongId) {
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
    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_TRUE(accessor.checkFinalized());

    int number_of_caught_exceptions = 0;
    try {
      daily_table.deleteRecord(record_3.getID() + 5);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
    }
    EXPECT_EQ(number_of_caught_exceptions, 0);  // nothing bad happens in case of wrong deletion
    EXPECT_EQ(accessor.getNextID(), 3);
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}

TEST (DailyTableTest, DeleteRecordTwice) {
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
    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_TRUE(accessor.checkFinalized());

    daily_table.deleteRecord(record_2.getID());
    EXPECT_EQ(accessor.getNextID(), 3);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 2);

    int number_of_caught_exceptions = 0;
    try {
      daily_table.deleteRecord(record_2.getID());
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
    }
    EXPECT_EQ(number_of_caught_exceptions, 0);  // nothing bad happens in case of twice deletion
    EXPECT_EQ(accessor.getNextID(), 3);
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_daily_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_daily_table_db_filename.c_str());
}


/* SQLite database testing */
// ----------------------------------------------------------------------------
TEST (SQLiteDatabaseTest, SingleTableOpenFromTwoHandlers) {
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

    std::string count_statement = "SELECT COUNT(*) FROM "
        "sqlite_master WHERE type == 'table' "
        "AND name != '";
    count_statement += cycle_accessor.getLastRowIdTableName();
    count_statement += "' AND name != '";
    count_statement += daily_accessor.getLastRowIdTableName();
    count_statement += "' AND name != 'sqlite_sequence';";
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
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    EXPECT_EQ(sqlite3_column_count(statement_handler), 1);
    int type = sqlite3_column_type(statement_handler, 0);
    EXPECT_EQ(type, SQLITE_INTEGER);
    int value = sqlite3_column_int(statement_handler, 0);
    EXPECT_EQ(value, 2);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    EXPECT_TRUE(cycle_accessor.checkFinalized());
    EXPECT_TRUE(daily_accessor.checkFinalized());
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_single_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_single_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_single_db_filename.c_str());
}

TEST (SQLiteDatabaseTest, TablePersistense) {
  std::string test_single_db_filename = "Test-SingleTable.db";
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
    mw::WrappedString s_name = "Имя слота";
    mw::WrappedString s_entry_description = "Тестовое описание слота";
    mw::WrappedString s_record_description = "Тестовая запись в таблице";
    MoneyValue_t s_entry_balance = 1000;
    MoneyValue_t s_record_balance = 500;
    mw::Status s_record_status(mw::SV_UNKNOWN);

    {  // open single database and fill Cycle_Table
      mw::CycleTable cycle_table(test_single_db_filename);
      EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::CycleTable> cycle_accessor(&cycle_table);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
      std::string cycle_table_name = cycle_accessor.getTableName();
      EXPECT_EQ(cycle_accessor.countRows(cycle_table_name), 0);
      cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
      cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
      cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
      cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
      cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
      EXPECT_EQ(cycle_accessor.countRows(cycle_table_name), 5);
      EXPECT_EQ(cycle_accessor.getNextID(), 5);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
    {  // open again, check count
      mw::CycleTable cycle_table(test_single_db_filename);
      EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::CycleTable> cycle_accessor(&cycle_table);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
      std::string cycle_table_name = cycle_accessor.getTableName();
      EXPECT_EQ(cycle_accessor.countRows(cycle_table_name), 5);
      EXPECT_EQ(cycle_accessor.getNextID(), 5);
      cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
      cycle_table.addEntry(s_name, s_entry_description, s_entry_balance);
      EXPECT_EQ(cycle_accessor.countRows(cycle_table_name), 7);
      EXPECT_EQ(cycle_accessor.getNextID(), 7);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);

    {  // open single database and fill Daily_Table
      mw::DailyTable daily_table(test_single_db_filename);
      EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::DailyTable> daily_accessor(&daily_table);
      EXPECT_TRUE(daily_accessor.checkFinalized());
      std::string daily_table_name = daily_accessor.getTableName();
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 0);
      daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
      daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
      daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 3);
      EXPECT_EQ(daily_accessor.getNextID(), 3);
      EXPECT_TRUE(daily_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
    {  // open again, check count
      mw::DailyTable daily_table(test_single_db_filename);
      EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::DailyTable> daily_accessor(&daily_table);
      EXPECT_TRUE(daily_accessor.checkFinalized());
      std::string daily_table_name = daily_accessor.getTableName();
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 3);
      EXPECT_EQ(daily_accessor.getNextID(), 3);
      daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
      daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 5);
      EXPECT_EQ(daily_accessor.getNextID(), 5);
      EXPECT_TRUE(daily_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_single_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_single_db_filename.c_str());
  }
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  remove(test_single_db_filename.c_str());
}


/* TableManager testing */
// ----------------------------------------------------------------------------
TEST (TableManagerTest, TableManagerInit) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
     mw::TableManager& table_manager = mw::TableManager::instance();
     EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
     EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
     EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(mw::TableManager::single_database_name.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(mw::TableManager::single_database_name.c_str());
  }
  // static instance deletion time is undefined.
  remove(mw::TableManager::single_database_name.c_str());
}


/* Main */
// ----------------------------------------------------------------------------
int main(int argc, char** argv) {
  //setlocale(LC_CTYPE, "");
  ::testing::InitGoogleTest(&argc, argv);
  return (RUN_ALL_TESTS());
}
