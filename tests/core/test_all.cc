/*
 * test_all.cc
 *
 *  Description: Unit-tests for core testing.
 *
 *  Created on: 28.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <locale.h>
#include <unistd.h>
#include "gtest/gtest.h"
#include "test_fixture.h"
#include "test_include_all.h"


// ----------------------------------------------
TEST (SimpleDemoTest, /*DISABLED_*/SimpleDemo) {
  /*std::thread simple_test([&] () {
    EXPECT_EQ(1.0, 1.0);
  });*/
  EXPECT_EQ(1.0, 1.0);
}


#if ENABLED_TIME_MEASURE_ONLY
/* Time consumption measurements */
// XXX(navigation): Time measurements
// ----------------------------------------------------------------------------
const std::string TimeMeasureFixture::test_db_filename = "Test-TimeMeasureFixture.db";

TEST (TimeMeasure, CreateCycleTable) {
  std::string test_cycle_table_db_filename = "Test-CycleTable.db";
  mw::CycleTable cycle_table(test_cycle_table_db_filename);
  remove(test_cycle_table_db_filename.c_str());
}

TEST_F (TimeMeasureFixture, AddEntry) {
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_description = "Тестовое описание слота";
  MoneyValue_t s_balance = 1000;
  this->getCycleTable().addEntry(s_name, s_description, s_balance);
}

TEST_F (TimeMeasureFixture, AddManyEntries) {
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_description = "Тестовое описание слота";
  MoneyValue_t s_balance = 1000;
  this->getCycleTable().addEntry(s_name, s_description, s_balance);
  this->getCycleTable().addEntry(s_name, s_description, s_balance);
  this->getCycleTable().addEntry(s_name, s_description, s_balance);
  this->getCycleTable().addEntry(s_name, s_description, s_balance);
  this->getCycleTable().addEntry(s_name, s_description, s_balance);
}

TEST_F (TimeMeasureFixture, ReadEntry) {
  ID_t id = rand() % TimeMeasureFixture::total_rows;
  this->getCycleTable().readEntry(id);
}

TEST_F (TimeMeasureFixture, UpdateEntry) {
  MoneyValue_t s_expense = -700;
  mw::WrappedString s_transaction_comment = "Расход на 700 единиц";
  ID_t id = rand() % TimeMeasureFixture::total_rows;
  this->getCycleTable().updateEntry(id, s_expense, s_transaction_comment);
}

TEST_F (TimeMeasureFixture, DeleteEntry) {
  ID_t id = rand() % TimeMeasureFixture::total_rows;
  this->getCycleTable().deleteEntry(id);
}

TEST_F (TimeMeasureFixture, DeleteManyEntries) {
  std::vector<ID_t> ids = this->getEntryIDs();
  for (ID_t& id : ids) {
    this->getCycleTable().deleteEntry(id);
  }
}

TEST_F (TimeMeasureFixture, DeleteManyEntriesByOneSQLstatement) {
  std::vector<ID_t> ids = this->getEntryIDs();
  this->getCycleTable().deleteEntries(ids);
}

// ----------------------------------------------
TEST (TimeMeasure, CreateDailyTable) {
  std::string test_daily_table_db_filename = "Test-DailyTable.db";
  mw::DailyTable daily_table(test_daily_table_db_filename);
  remove(test_daily_table_db_filename.c_str());
}

TEST_F (TimeMeasureFixture, AddRecord) {
  MoneyValue_t s_balance = 1000;
  mw::WrappedString s_description = "Тестовая запись в таблице";
  mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
  this->getDailyTable().addRecord(s_balance, s_description, s_status);
}

TEST_F (TimeMeasureFixture, AddManyRecord) {
  MoneyValue_t s_balance = 1000;
  mw::WrappedString s_description = "Тестовая запись в таблице";
  mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
  this->getDailyTable().addRecord(s_balance, s_description, s_status);
  this->getDailyTable().addRecord(s_balance, s_description, s_status);
  this->getDailyTable().addRecord(s_balance, s_description, s_status);
  this->getDailyTable().addRecord(s_balance, s_description, s_status);
  this->getDailyTable().addRecord(s_balance, s_description, s_status);
}

TEST_F (TimeMeasureFixture, ReadRecord) {
  ID_t id = rand() % TimeMeasureFixture::total_rows;
  this->getDailyTable().readRecord(id);
}

TEST_F (TimeMeasureFixture, DeleteRecord) {
  ID_t id = rand() % TimeMeasureFixture::total_rows;
  this->getDailyTable().deleteRecord(id);
}

TEST_F (TimeMeasureFixture, DeleteManyRecords) {
  std::vector<ID_t> ids = this->getRecordIDs();
  for (ID_t& id : ids) {
    this->getDailyTable().deleteRecord(id);
  }
}

TEST_F (TimeMeasureFixture, DeleteManyRecordsByOneSQLstatement) {
  std::vector<ID_t> ids = this->getRecordIDs();
  this->getDailyTable().deleteRecords(ids);
}

// ----------------------------------------------
TEST (TimeMeasure, TableManagerInit) {
  mw::TableManager table_manager;
  remove(mw::TableManager::single_database_name.c_str());
}

TEST_F (TableManagerTimeMeasureFixture, TableManagerAdd) {
  mw::WrappedString s_name = "Имя слота";
  MoneyValue_t s_balance = 1000;
  mw::WrappedString s_description = "Тестовая запись в таблице";
  this->getTableManager().add(s_name, s_description, s_balance);
}

TEST_F (TableManagerTimeMeasureFixture, TableManagerUpdate) {
  mw::WrappedString s_update_description = "Расход на 700 единиц";
  MoneyValue_t s_expense = -700;
  ID_t id = rand() % TableManagerTimeMeasureFixture::total_rows;
  this->getTableManager().update(id, s_expense, s_update_description);
}

TEST_F (TableManagerTimeMeasureFixture, TableManagerMultipleUpdate) {
  mw::WrappedString s_update_description = "Расход на 700 единиц";
  MoneyValue_t s_expense = -700;
  std::vector<ID_t> ids = this->getEntryIDs();
  for (ID_t& id : ids) {
    this->getTableManager().update(id, s_expense, s_update_description);
  }
}

TEST_F (TableManagerTimeMeasureFixture, TableManagerRemove) {
  mw::WrappedString s_update_description = "Расход на 700 единиц";
  MoneyValue_t s_expense = -700;
  ID_t id = rand() % TableManagerTimeMeasureFixture::total_rows;
  this->getTableManager().update(id, s_expense, s_update_description);
  this->getTableManager().update(id, s_expense, s_update_description);
  this->getTableManager().remove(id);
}


#else  // ENABLED_TIME_MEASURE_ONLY


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

static int getInt(
    const std::string& table_name,
    const std::string& statement,
    const DB_Handler& handler) {
  int nByte = static_cast<int>(statement.length());
  DB_Statement statement_handler = nullptr;
  int result = sqlite3_prepare_v2(
      handler,
      statement.c_str(),
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
  return (value);
}

static int countRows(const std::string& table_name, const DB_Handler& handler) {
  std::string count_statement = "SELECT COUNT(*) FROM '";
  count_statement += table_name;
  count_statement += "';";
  return (getInt(table_name, count_statement, handler));
}


/* CycleTable testing */
// XXX(navigation): CycleTable testing
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
#include <iostream>
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_UNKNOWN);
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
    EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::RecordStatusValue::RSV_UNKNOWN));
    mw::RecordStatus status(raw_status);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_UNKNOWN);
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
      EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::RecordStatusValue::RSV_UNKNOWN));
      mw::RecordStatus status(raw_status);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_UNKNOWN);
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
      EXPECT_EQ(e.error(), TABLE_ASSERTION_ERROR_CODE);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_EXPENSE);
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

TEST (CycleTableTest, UpdateEntryWrongId) {
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_EXPENSE);
    EXPECT_EQ(entry.getStatus(), s_status);

    int number_of_caught_exceptions = 0;
    try {
      mw::Entry updated_entry =
          cycle_table.updateEntry(entry.getID() + 5, s_expense, s_transaction_comment);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
      EXPECT_EQ(e.error(), TABLE_ASSERTION_ERROR_CODE);
    }
    EXPECT_EQ(number_of_caught_exceptions, 1);

    try {
      mw::Entry read_entry = cycle_table.readEntry(entry.getID() + 5);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
      EXPECT_EQ(e.error(), TABLE_ASSERTION_ERROR_CODE);
    }
    EXPECT_EQ(number_of_caught_exceptions, 2);

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

TEST (CycleTableTest, RollbackEntry) {
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
    mw::Record record = entry.make_record();
    EXPECT_TRUE(accessor.checkFinalized());

    MoneyValue_t s_expense = -700;
    mw::WrappedString s_transaction_comment = "Расход на 700 единиц";
    cycle_table.updateEntry(entry.getID(), s_expense, s_transaction_comment);
    cycle_table.rollbackEntry(entry.getID(), s_expense, record);
    mw::Entry read_entry = cycle_table.readEntry(entry.getID());
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

TEST (CycleTableTest, DeleteEntry) {
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
    mw::Entry entry_1 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_2 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_3 = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), entry_3.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    cycle_table.deleteEntry(entry_3.getID());
    EXPECT_EQ(accessor.getNextID(), 2);
    EXPECT_EQ(accessor.getNextID(), entry_2.getID() + 1);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 2);

    cycle_table.deleteEntry(entry_1.getID());
    EXPECT_EQ(accessor.getNextID(), 2);
    EXPECT_EQ(accessor.getNextID(), entry_2.getID() + 1);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);

    cycle_table.deleteEntry(entry_2.getID());
    EXPECT_EQ(accessor.getNextID(), 0);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);

    mw::Entry record_11 = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_EQ(accessor.getNextID(), 1);
    EXPECT_EQ(accessor.getNextID(), record_11.getID() + 1);
    mw::Entry record_21 = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_EQ(accessor.getNextID(), 2);
    EXPECT_EQ(accessor.getNextID(), record_21.getID() + 1);
    mw::Entry record_31 = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_31.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 3);

    cycle_table.deleteEntry(record_21.getID());
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_31.getID() + 1);
    cycle_table.deleteEntry(record_31.getID());
    EXPECT_EQ(accessor.getNextID(), 1);
    EXPECT_EQ(accessor.getNextID(), record_11.getID() + 1);
    cycle_table.deleteEntry(record_11.getID());
    EXPECT_EQ(accessor.getNextID(), 0);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);
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

TEST (CycleTableTest, DeleteEntryWrongId) {
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
    mw::Entry entry_1 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_2 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_3 = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), entry_3.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    int number_of_caught_exceptions = 0;
    try {
      cycle_table.deleteEntry(entry_3.getID() + 5);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
    }
    EXPECT_EQ(number_of_caught_exceptions, 0);  // nothing bad happens in case of wrong deletion
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), entry_3.getID() + 1);
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

TEST (CycleTableTest, DeleteEntryTwice) {
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
    mw::Entry entry_1 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_2 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_3 = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), entry_3.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    cycle_table.deleteEntry(entry_2.getID());
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), entry_3.getID() + 1);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 2);

    int number_of_caught_exceptions = 0;
    try {
      cycle_table.deleteEntry(entry_2.getID());
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
    }
    EXPECT_EQ(number_of_caught_exceptions, 0);  // nothing bad happens in case of twice deletion
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), entry_3.getID() + 1);
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

TEST (CycleTableTest, DeleteManyEntries) {
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
    std::vector<mw::Entry> entries;
    int total_entries = 10;
    entries.reserve(total_entries);
    for (int index = 0; index < total_entries; ++index) {
      mw::Entry entry = cycle_table.addEntry(s_name, s_description, s_balance);
      entries.push_back(entry);
    }
    EXPECT_EQ(accessor.getNextID(), total_entries);
    EXPECT_TRUE(accessor.checkFinalized());

    for (std::vector<mw::Entry>::const_reverse_iterator it = entries.rbegin();
         it != entries.rend();
         ++it) {
      cycle_table.deleteEntry(it->getID());
      --total_entries;
      EXPECT_EQ(accessor.getNextID(), total_entries);
      int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
      EXPECT_EQ(rows, total_entries);
    }
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);
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

TEST (CycleTableTest, DeleteManyEntriesByOneSQLstatement) {
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
    std::vector<ID_t> entry_ids;
    int total_entries = 10;
    entry_ids.reserve(total_entries);
    for (int index = 0; index < total_entries; ++index) {
      mw::Entry entry = cycle_table.addEntry(s_name, s_description, s_balance);
      entry_ids.push_back(entry.getID());
    }
    EXPECT_EQ(accessor.getNextID(), total_entries);
    EXPECT_TRUE(accessor.checkFinalized());

    cycle_table.deleteEntries(entry_ids);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);

    mw::Entry entry_1 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_2 = cycle_table.addEntry(s_name, s_description, s_balance);
    mw::Entry entry_3 = cycle_table.addEntry(s_name, s_description, s_balance);
    EXPECT_EQ(accessor.getNextID(), 3);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 3);
    entry_ids.clear();
    for (int index = 0; index < total_entries; ++index) {
      mw::Entry entry = cycle_table.addEntry(s_name, s_description, s_balance);
      entry_ids.push_back(entry.getID());
    }
    EXPECT_EQ(accessor.getNextID(), 13);
    cycle_table.deleteEntry(entry_3.getID());
    EXPECT_EQ(accessor.getNextID(), 13);
    cycle_table.deleteEntry(entry_2.getID());
    EXPECT_EQ(accessor.getNextID(), 13);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 11);
    cycle_table.deleteEntries(entry_ids);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);
    EXPECT_EQ(accessor.getNextID(), 1);
    EXPECT_EQ(accessor.getNextID(), entry_1.getID() + 1);
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


/* DailyTable testing */
// XXX(navigation): DailyTable testing
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
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
    EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::RecordStatusValue::RSV_INCOME));
    mw::RecordStatus status(raw_status);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
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
      EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::RecordStatusValue::RSV_INCOME));
      mw::RecordStatus status(raw_status);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
    mw::Record record = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_TRUE(accessor.checkFinalized());

    int number_of_caught_exceptions = 0;
    try {
      mw::Record read_record = daily_table.readRecord(record.getID() + 5);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
      EXPECT_EQ(e.error(), TABLE_ASSERTION_ERROR_CODE);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_3.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    daily_table.deleteRecord(record_3.getID());
    EXPECT_EQ(accessor.getNextID(), 2);
    EXPECT_EQ(accessor.getNextID(), record_2.getID() + 1);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 2);

    daily_table.deleteRecord(record_1.getID());
    EXPECT_EQ(accessor.getNextID(), 2);
    EXPECT_EQ(accessor.getNextID(), record_2.getID() + 1);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);

    daily_table.deleteRecord(record_2.getID());
    EXPECT_EQ(accessor.getNextID(), 0);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);

    mw::Record record_11 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 1);
    EXPECT_EQ(accessor.getNextID(), record_11.getID() + 1);
    mw::Record record_21 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 2);
    EXPECT_EQ(accessor.getNextID(), record_21.getID() + 1);
    mw::Record record_31 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_31.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 3);

    daily_table.deleteRecord(record_21.getID());
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_31.getID() + 1);
    daily_table.deleteRecord(record_31.getID());
    EXPECT_EQ(accessor.getNextID(), 1);
    EXPECT_EQ(accessor.getNextID(), record_11.getID() + 1);
    daily_table.deleteRecord(record_11.getID());
    EXPECT_EQ(accessor.getNextID(), 0);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_3.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    int number_of_caught_exceptions = 0;
    try {
      daily_table.deleteRecord(record_3.getID() + 5);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
    }
    EXPECT_EQ(number_of_caught_exceptions, 0);  // nothing bad happens in case of wrong deletion
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_3.getID() + 1);
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
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_3.getID() + 1);
    EXPECT_TRUE(accessor.checkFinalized());

    daily_table.deleteRecord(record_2.getID());
    EXPECT_EQ(accessor.getNextID(), 3);
    EXPECT_EQ(accessor.getNextID(), record_3.getID() + 1);
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
    EXPECT_EQ(accessor.getNextID(), record_3.getID() + 1);
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

TEST (DailyTableTest, DeleteManyRecords) {
  std::string test_daily_table_db_filename = "Test-DailyTable.db";
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
    mw::DailyTable daily_table(test_daily_table_db_filename);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::DailyTable> accessor(&daily_table);
    EXPECT_TRUE(accessor.checkFinalized());
    MoneyValue_t s_balance = 1000;
    mw::WrappedString s_description = "Тестовая запись в таблице";
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
    std::vector<mw::Record> records;
    int total_records = 10;
    records.reserve(total_records);
    for (int index = 0; index < total_records; ++index) {
      mw::Record record = daily_table.addRecord(s_balance, s_description, s_status);
      records.push_back(record);
    }
    EXPECT_EQ(accessor.getNextID(), total_records);
    EXPECT_TRUE(accessor.checkFinalized());

    for (std::vector<mw::Record>::const_reverse_iterator it = records.rbegin();
         it != records.rend();
         ++it) {
      daily_table.deleteRecord(it->getID());
      --total_records;
      EXPECT_EQ(accessor.getNextID(), total_records);
      int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
      EXPECT_EQ(rows, total_records);
    }
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);
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

TEST (DailyTableTest, DeleteManyRecordsByOneSQLstatement) {
  std::string test_daily_table_db_filename = "Test-DailyTable.db";
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  try {
    mw::DailyTable daily_table(test_daily_table_db_filename);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::DailyTable> accessor(&daily_table);
    EXPECT_TRUE(accessor.checkFinalized());
    MoneyValue_t s_balance = 1000;
    mw::WrappedString s_description = "Тестовая запись в таблице";
    mw::RecordStatus s_status(mw::RecordStatusValue::RSV_INCOME);
    std::vector<ID_t> record_ids;
    int total_records = 10;
    record_ids.reserve(total_records);
    for (int index = 0; index < total_records; ++index) {
      mw::Record record = daily_table.addRecord(s_balance, s_description, s_status);
      record_ids.push_back(record.getID());
    }
    EXPECT_EQ(accessor.getNextID(), total_records);
    EXPECT_TRUE(accessor.checkFinalized());

    daily_table.deleteRecords(record_ids);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0);
    EXPECT_EQ(accessor.getNextID(), 0);

    mw::Record record_1 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_2 = daily_table.addRecord(s_balance, s_description, s_status);
    mw::Record record_3 = daily_table.addRecord(s_balance, s_description, s_status);
    EXPECT_EQ(accessor.getNextID(), 3);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 3);
    record_ids.clear();
    for (int index = 0; index < total_records; ++index) {
      mw::Record record = daily_table.addRecord(s_balance, s_description, s_status);
      record_ids.push_back(record.getID());
    }
    EXPECT_EQ(accessor.getNextID(), 13);
    daily_table.deleteRecord(record_3.getID());
    EXPECT_EQ(accessor.getNextID(), 13);
    daily_table.deleteRecord(record_2.getID());
    EXPECT_EQ(accessor.getNextID(), 13);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 11);
    daily_table.deleteRecords(record_ids);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);
    EXPECT_EQ(accessor.getNextID(), 1);
    EXPECT_EQ(accessor.getNextID(), record_1.getID() + 1);
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


/* PolicyTable testing */
// XXX(navigation): PolicyTable testing
// ----------------------------------------------------------------------------
TEST (PolicyTableTest, CreatePolicyTable) {
  std::string test_policy_table_db_filename = "Test-PolicyTable.db";
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  try {
    mw::PolicyTable policy_table(test_policy_table_db_filename);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::PolicyTable> accessor(&policy_table);
    EXPECT_TRUE(accessor.checkFinalized());
    int db_file_exists = access(test_policy_table_db_filename.c_str(), F_OK);
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
    remove(test_policy_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(test_policy_table_db_filename.c_str());
}

TEST (PolicyTableTest, AddPolicy) {
  std::string test_policy_table_db_filename = "Test-PolicyTable.db";
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  try {
    mw::PolicyTable policy_table(test_policy_table_db_filename);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::PolicyTable> accessor(&policy_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя политики";
    mw::WrappedString s_description = "Тестовая запись в таблице";
    PolicyRatio_t s_ratio = 20;
    ID_t s_source_entry_id = 2;
    ID_t s_destination_entry_id = 1;
    int s_hours_period = 168;
    mw::PolicyStatus s_status(mw::PolicyStatusValue::PSV_ENABLED);
    mw::Policy policy = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(policy.getID(), accessor.getNextID() - 1);
    EXPECT_STREQ(policy.getName().c_str(), s_name.c_str());
    EXPECT_STREQ(policy.getDescription().c_str(), s_description.c_str());
    EXPECT_EQ(policy.getRatio(), s_ratio);
    EXPECT_EQ(policy.getSourceID(), s_source_entry_id);
    EXPECT_EQ(policy.getDestinationID(), s_destination_entry_id);
    EXPECT_EQ(policy.getPeriod(), s_hours_period);
    EXPECT_EQ(policy.getStatus(), s_status);

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
    EXPECT_EQ(policy.getID(), id);
    const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
    mw::WrappedString name(raw_name);
    EXPECT_STREQ(policy.getName().c_str(), name.c_str());
    const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
    mw::WrappedString description(raw_description);
    EXPECT_STREQ(policy.getDescription().c_str(), description.c_str());
    PolicyRatio_t ratio = sqlite3_column_int64(statement_handler, 3);
    EXPECT_EQ(policy.getRatio(), ratio);
    ID_t source_entry_id = sqlite3_column_int64(statement_handler, 4);
    EXPECT_EQ(policy.getSourceID(), source_entry_id);
    ID_t destination_entry_id = sqlite3_column_int64(statement_handler, 5);
    EXPECT_EQ(policy.getDestinationID(), destination_entry_id);
    int hours_period = sqlite3_column_int(statement_handler, 6);
    EXPECT_EQ(policy.getPeriod(), hours_period);
    sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 9);
    EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::PolicyStatusValue::PSV_ENABLED));
    mw::PolicyStatus status(raw_status);
    EXPECT_EQ(policy.getStatus(), status);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(test_policy_table_db_filename.c_str());
}

TEST (PolicyTableTest, AddManyPolicies) {
  std::string test_policy_table_db_filename = "Test-PolicyTable.db";
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  try {
    mw::PolicyTable policy_table(test_policy_table_db_filename);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::PolicyTable> accessor(&policy_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя политики";
    mw::WrappedString s_description = "Тестовая запись в таблице";
    PolicyRatio_t s_ratio = 20;
    ID_t s_source_entry_id = 2;
    ID_t s_destination_entry_id = 1;
    int s_hours_period = 168;
    mw::PolicyStatus s_status(mw::PolicyStatusValue::PSV_ENABLED);
    std::vector<mw::Policy> policys;
    policys.reserve(10);
    EXPECT_EQ(policys.size(), 0);
    mw::Policy policy_1 = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(policy_1.getID(), accessor.getNextID() - 1);
    policys.push_back(policy_1);
    mw::Policy policy_2 = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(policy_2.getID(), accessor.getNextID() - 1);
    policys.push_back(policy_2);
    mw::Policy policy_3 = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(policy_3.getID(), accessor.getNextID() - 1);
    policys.push_back(policy_3);
    mw::Policy policy_4 = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(policy_4.getID(), accessor.getNextID() - 1);
    policys.push_back(policy_4);
    mw::Policy policy_5 = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(policy_5.getID(), accessor.getNextID() - 1);
    policys.push_back(policy_5);
    EXPECT_EQ(accessor.getNextID(), 5);
    EXPECT_EQ(policys.size(), 5);

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
    for (auto it = policys.begin(); it != policys.end(); ++it) {
      loop_result = sqlite3_step(statement_handler);
      EXPECT_EQ(loop_result, SQLITE_ROW);
      ID_t id = sqlite3_column_int64(statement_handler, 0);
      EXPECT_EQ(it->getID(), id);
      const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
      mw::WrappedString name(raw_name);
      EXPECT_STREQ(it->getName().c_str(), name.c_str());
      const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
      mw::WrappedString description(raw_description);
      EXPECT_STREQ(it->getDescription().c_str(), description.c_str());
      PolicyRatio_t ratio = sqlite3_column_int64(statement_handler, 3);
      EXPECT_EQ(it->getRatio(), ratio);
      ID_t source_entry_id = sqlite3_column_int64(statement_handler, 4);
      EXPECT_EQ(it->getSourceID(), source_entry_id);
      ID_t destination_entry_id = sqlite3_column_int64(statement_handler, 5);
      EXPECT_EQ(it->getDestinationID(), destination_entry_id);
      int hours_period = sqlite3_column_int(statement_handler, 6);
      EXPECT_EQ(it->getPeriod(), hours_period);
      sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 9);
      EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::PolicyStatusValue::PSV_ENABLED));
      mw::PolicyStatus status(raw_status);
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
    remove(test_policy_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(test_policy_table_db_filename.c_str());
}

TEST (PolicyTableTest, ReadPolicy) {
  std::string test_policy_table_db_filename = "Test-PolicyTable.db";
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  try {
    mw::PolicyTable policy_table(test_policy_table_db_filename);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::PolicyTable> accessor(&policy_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя политики";
    mw::WrappedString s_description = "Тестовая запись в таблице";
    PolicyRatio_t s_ratio = 20;
    ID_t s_source_entry_id = 2;
    ID_t s_destination_entry_id = 1;
    int s_hours_period = 168;
    mw::PolicyStatus s_status(mw::PolicyStatusValue::PSV_ENABLED);
    mw::Policy policy = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());

    std::shared_ptr<mw::DateTime> ptr_datetime;
    mw::Policy read_policy = policy_table.readPolicy(policy.getID(), ptr_datetime);
    EXPECT_TRUE(accessor.checkFinalized());
    EXPECT_EQ(read_policy.getID(), policy.getID());
    EXPECT_STREQ(read_policy.getName().c_str(), policy.getName().c_str());
    EXPECT_STREQ(read_policy.getDescription().c_str(), policy.getDescription().c_str());
    EXPECT_EQ(read_policy.getRatio(), policy.getRatio());
    EXPECT_EQ(read_policy.getSourceID(), policy.getSourceID());
    EXPECT_EQ(read_policy.getDestinationID(), policy.getDestinationID());
    EXPECT_EQ(read_policy.getPeriod(), policy.getPeriod());
    EXPECT_EQ(read_policy.getStatus(), policy.getStatus());
    EXPECT_TRUE(accessor.checkFinalized());

  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(test_policy_table_db_filename.c_str());
}

TEST (PolicyTableTest, ReadPolicyWrongId) {
  std::string test_policy_table_db_filename = "Test-PolicyTable.db";
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  try {
    mw::PolicyTable policy_table(test_policy_table_db_filename);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::PolicyTable> accessor(&policy_table);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::WrappedString s_name = "Имя политики";
    mw::WrappedString s_description = "Тестовая запись в таблице";
    PolicyRatio_t s_ratio = 20;
    ID_t s_source_entry_id = 2;
    ID_t s_destination_entry_id = 1;
    int s_hours_period = 168;
    mw::PolicyStatus s_status(mw::PolicyStatusValue::PSV_ENABLED);
    mw::Policy policy = policy_table.addPolicy(s_name, s_description, s_ratio, s_source_entry_id, s_destination_entry_id, s_hours_period, s_status);
    EXPECT_TRUE(accessor.checkFinalized());

    int number_of_caught_exceptions = 0;
    try {
      std::shared_ptr<mw::DateTime> ptr_datetime;
      mw::Policy read_policy = policy_table.readPolicy(policy.getID() + 5, ptr_datetime);
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
      EXPECT_EQ(e.error(), TABLE_ASSERTION_ERROR_CODE);
    }
    EXPECT_EQ(number_of_caught_exceptions, 1);

  } catch (mw::TableException& e) {
    WRN("Handled table exception in unit-tests: ["%s"]! Error code: %s.",
        e.what(), intToSQLiteError(e.error()));
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  } catch (...) {
    WRN("Got exception!");
    EXPECT_TRUE(false);
    remove(test_policy_table_db_filename.c_str());
  }
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(test_policy_table_db_filename.c_str());
}


/* SQLite database testing */
// XXX(navigation): SQLite database testing
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
    mw::RecordStatus unknown_status(mw::RecordStatusValue::RSV_UNKNOWN);
    mw::RecordStatus s_entry_status(mw::RecordStatusValue::RSV_EXPENSE);
    mw::RecordStatus s_record_status(mw::RecordStatusValue::RSV_INCOME);

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
        " AND name != 'sqlite_sequence';";
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
    mw::RecordStatus s_record_status(mw::RecordStatusValue::RSV_UNKNOWN);

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
    {  // open again, delete entries
      mw::CycleTable cycle_table(test_single_db_filename);
      EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::CycleTable> cycle_accessor(&cycle_table);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
      std::string cycle_table_name = cycle_accessor.getTableName();
      cycle_table.deleteEntry(2);
      EXPECT_EQ(cycle_accessor.countRows(cycle_table_name), 6);
      EXPECT_EQ(cycle_accessor.getNextID(), 7);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
    {  // open again, check ids
      mw::CycleTable cycle_table(test_single_db_filename);
      EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::CycleTable> cycle_accessor(&cycle_table);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
      std::string cycle_table_name = cycle_accessor.getTableName();
      std::vector<ID_t> entry_ids = {4, 6, 3, 5};
      cycle_table.deleteEntries(entry_ids);
      EXPECT_EQ(cycle_accessor.countRows(cycle_table_name), 2);
      EXPECT_EQ(cycle_accessor.getNextID(), 1 + 1);
      EXPECT_TRUE(cycle_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);

    // ------------------------------------------------------------------------
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
    {  // open again, delete records
      mw::DailyTable daily_table(test_single_db_filename);
      EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::DailyTable> daily_accessor(&daily_table);
      EXPECT_TRUE(daily_accessor.checkFinalized());
      std::string daily_table_name = daily_accessor.getTableName();
      daily_table.deleteRecord(2);
      daily_table.deleteRecord(1);
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 3);
      EXPECT_EQ(daily_accessor.getNextID(), 5);
      EXPECT_TRUE(daily_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
    {  // open again, check ids
      mw::DailyTable daily_table(test_single_db_filename);
      EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::DailyTable> daily_accessor(&daily_table);
      EXPECT_TRUE(daily_accessor.checkFinalized());
      std::string daily_table_name = daily_accessor.getTableName();
      std::vector<ID_t> record_ids = {4, 3};
      daily_table.deleteRecords(record_ids);
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 1);
      EXPECT_EQ(daily_accessor.getNextID(), 0 + 1);
      daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
      daily_table.addRecord(s_record_balance, s_record_description, s_record_status);
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 3);
      EXPECT_EQ(daily_accessor.getNextID(), 3);
      EXPECT_TRUE(daily_accessor.checkFinalized());
    }
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
    {  // open again, check ids
      mw::DailyTable daily_table(test_single_db_filename);
      EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
      mw::TestAccessTable<mw::DailyTable> daily_accessor(&daily_table);
      EXPECT_TRUE(daily_accessor.checkFinalized());
      std::string daily_table_name = daily_accessor.getTableName();
      daily_table.deleteRecord(2);
      daily_table.deleteRecord(1);
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 1);
      EXPECT_EQ(daily_accessor.getNextID(), 0 + 1);
      daily_table.deleteRecord(0);
      EXPECT_EQ(daily_accessor.countRows(daily_table_name), 0);
      EXPECT_EQ(daily_accessor.getNextID(), 0);
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
// XXX(navigation): TableManager testing
// ----------------------------------------------------------------------------
TEST (TableManagerTest, TableManagerInit) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  try {
     mw::TableManager table_manager;
     EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
     EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
     EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
     EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
     mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
     EXPECT_TRUE(accessor.checkFinalized());
     int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
     EXPECT_EQ(rows, 0);

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerAdd) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  MoneyValue_t s_entry_balance = 1000;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id = init.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

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
    EXPECT_EQ(entry_id, id);
    std::string records_table_name(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1)));
    std::string check_string = mw::TableManager::records_table_name_prefix + std::to_string(entry_id);
    EXPECT_STREQ(records_table_name.c_str(), check_string.c_str());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    rows = countRows(records_table_name, accessor.getDbHandler());
    EXPECT_EQ(rows, 1);

    statement_handler = nullptr;
    value_statement = "SELECT * FROM \'";
    value_statement += records_table_name;
    value_statement += "\';";
    nByte = static_cast<int>(value_statement.length());
    result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        value_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t record_id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(record_id, init.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerUpdate) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  mw::WrappedString s_update_description = "Расход на 700 единиц";
  MoneyValue_t s_entry_balance = 1000;
  MoneyValue_t s_expense = -700;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id = init.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

    mw::Record record = table_manager.update(entry_id, s_expense, s_update_description);
    ID_t record_id = record.getID();
    std::string records_table_name = mw::TableManager::records_table_name_prefix + std::to_string(entry_id);

    DB_Statement statement_handler = nullptr;
    std::string check_statement = "SELECT * FROM '";
    check_statement += records_table_name;
    check_statement += "';";
    int nByte = static_cast<int>(check_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id, init.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id, record_id);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerMultipleUpdate) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  mw::WrappedString s_update_description = "Расход на 700 единиц";
  MoneyValue_t s_entry_balance = 1000;
  MoneyValue_t s_expense = -700;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    table_manager.add(s_name, s_entry_description, s_entry_balance);
    std::pair<mw::Entry, mw::Record> init_2 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_2 = init_2.first.getID();
    std::pair<mw::Entry, mw::Record> init_3 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_3 = init_3.first.getID();
    table_manager.add(s_name, s_entry_description, s_entry_balance);
    std::pair<mw::Entry, mw::Record> init_5 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_5 = init_5.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

    int rows = countRows(table_manager.getCycleTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 5);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0 + 5);  // one row for initial record per entry, 5 entries

    std::string count_statement = "SELECT COUNT(*) FROM "
        "sqlite_master WHERE type == 'table' "
        "AND name != '";
    count_statement += accessor.getTableName();
    count_statement += "' AND name != '" + table_manager.getCycleTableName();
    count_statement += "' AND name != '" + table_manager.getDailyTableName();
    count_statement += "' AND name != '" + table_manager.getPolicyTableName();
    count_statement += "' AND name != 'sqlite_sequence';";
    int nByte = static_cast<int>(count_statement.length());
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
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
    // 5 Records tables
    EXPECT_EQ(value, 5);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 5);

    table_manager.update(entry_id_2, s_expense, s_update_description);
    mw::Record record_3_0 = table_manager.update(entry_id_3, s_expense, s_update_description);
    ID_t record_id_3_0 = record_3_0.getID();
    table_manager.update(entry_id_5, s_expense, s_update_description);
    mw::Record record_3_1 = table_manager.update(entry_id_3, s_expense, s_update_description);
    ID_t record_id_3_1 = record_3_1.getID();
    mw::Record record_3_2 = table_manager.update(entry_id_3, s_expense, s_update_description);
    ID_t record_id_3_2 = record_3_2.getID();
    std::string records_table_name_2 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_2);
    std::string records_table_name_3 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_3);
    std::string records_table_name_5 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_5);

    rows = countRows(records_table_name_2, accessor.getDbHandler());
    EXPECT_EQ(rows, 1 + 1);  // one row for initial record
    rows = countRows(records_table_name_3, accessor.getDbHandler());
    EXPECT_EQ(rows, 3 + 1);  // one row for initial record
    rows = countRows(records_table_name_5, accessor.getDbHandler());
    EXPECT_EQ(rows, 1 + 1);  // one row for initial record

    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 5 + 5);  // one row for initial record per entry, 5 entries

    statement_handler = nullptr;
    std::string check_statement = "SELECT * FROM '";
    check_statement += records_table_name_3;
    check_statement += "';";
    nByte = static_cast<int>(check_statement.length());
    result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_init = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_init, init_3.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_0 = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_0, record_id_3_0);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_1 = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_1, record_id_3_1);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_2 = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_2, record_id_3_2);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerRemove) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  mw::WrappedString s_update_description = "Расход на 700 единиц";
  MoneyValue_t s_entry_balance = 1000;
  MoneyValue_t s_expense = -700;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    table_manager.add(s_name, s_entry_description, s_entry_balance);
    std::pair<mw::Entry, mw::Record> init_2 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_2 = init_2.first.getID();
    std::pair<mw::Entry, mw::Record> init_3 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_3 = init_3.first.getID();
    table_manager.add(s_name, s_entry_description, s_entry_balance);
    std::pair<mw::Entry, mw::Record> init_5 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_5 = init_5.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

    table_manager.update(entry_id_2, s_expense, s_update_description);  // 0 record id
    table_manager.update(entry_id_3, s_expense, s_update_description);  // 1 record id
    table_manager.update(entry_id_3, s_expense, s_update_description);  // 2 record id
    table_manager.update(entry_id_5, s_expense, s_update_description);  // 3 record id
    table_manager.update(entry_id_3, s_expense, s_update_description);  // 4 record id
    std::string records_table_name_2 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_2);
    std::string records_table_name_3 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_3);
    std::string records_table_name_5 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_5);

    table_manager.remove(entry_id_3);
    int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 4);

    rows = countRows(table_manager.getCycleTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 4);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 2 + 5 - 1);  // 5 entries - 5 init records, -1 entry - -1 init record

    table_manager.remove(entry_id_5);
    rows = countRows(accessor.getTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 3);

    rows = countRows(table_manager.getCycleTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 3);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1 + 5 - 2);  // 5 entries - 5 init records, -2 entries - -2 init records

    std::string count_statement = "SELECT COUNT(*) FROM "
        "sqlite_master WHERE type == 'table' "
        "AND name != '";
    count_statement += accessor.getTableName();
    count_statement += "' AND name != '" + table_manager.getCycleTableName();
    count_statement += "' AND name != '" + table_manager.getDailyTableName();
    count_statement += "' AND name != '" + table_manager.getPolicyTableName();
    count_statement += "' AND name != 'sqlite_sequence';";
    int nByte = static_cast<int>(count_statement.length());
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
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
    // 3 Records tables
    EXPECT_EQ(value, 3);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerRemoveWrongId) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  mw::WrappedString s_update_description = "Расход на 700 единиц";
  MoneyValue_t s_entry_balance = 1000;
  MoneyValue_t s_expense = -700;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    table_manager.add(s_name, s_entry_description, s_entry_balance);
    std::pair<mw::Entry, mw::Record> init_2 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_2 = init_2.first.getID();
    std::pair<mw::Entry, mw::Record> init_3 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_3 = init_3.first.getID();
    table_manager.add(s_name, s_entry_description, s_entry_balance);
    std::pair<mw::Entry, mw::Record> init_5 = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id_5 = init_5.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

    table_manager.update(entry_id_2, s_expense, s_update_description);  // 0 record id
    table_manager.update(entry_id_3, s_expense, s_update_description);  // 1 record id
    table_manager.update(entry_id_3, s_expense, s_update_description);  // 2 record id
    table_manager.update(entry_id_5, s_expense, s_update_description);  // 3 record id
    table_manager.update(entry_id_3, s_expense, s_update_description);  // 4 record id
    std::string records_table_name_2 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_2);
    std::string records_table_name_3 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_3);
    std::string records_table_name_5 = mw::TableManager::records_table_name_prefix + std::to_string(entry_id_5);

    int number_of_caught_exceptions = 0;
    try {
      table_manager.remove(entry_id_5 + 5);
      int rows = countRows(accessor.getTableName(), accessor.getDbHandler());
      EXPECT_EQ(rows, 5);
      rows = countRows(table_manager.getCycleTableName(), accessor.getDbHandler());
      EXPECT_EQ(rows, 5);
      rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
      EXPECT_EQ(rows, 5 + 5);  // 5 updates
    } catch (mw::TableException& e) {
      ++number_of_caught_exceptions;
      EXPECT_EQ(e.error(), SQLITE_ERROR);
    }
    EXPECT_EQ(number_of_caught_exceptions, 1);

    std::string count_statement = "SELECT COUNT(*) FROM "
        "sqlite_master WHERE type == 'table' "
        "AND name != '";
    count_statement += accessor.getTableName();
    count_statement += "' AND name != '" + table_manager.getCycleTableName();
    count_statement += "' AND name != '" + table_manager.getDailyTableName();
    count_statement += "' AND name != '" + table_manager.getPolicyTableName();
    count_statement += "' AND name != 'sqlite_sequence';";
    int nByte = static_cast<int>(count_statement.length());
    DB_Statement statement_handler = nullptr;
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
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
    // 3 Records tables
    EXPECT_EQ(value, 5);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerUndo) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  mw::WrappedString s_expense_description = "Расход на 700 единиц";
  mw::WrappedString s_income_description = "Доход в 1100 единиц";
  MoneyValue_t s_entry_balance = 1000;
  MoneyValue_t s_expense = -700;
  MoneyValue_t s_income = 1100;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id = init.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

    std::string records_table_name = mw::TableManager::records_table_name_prefix + std::to_string(entry_id);
    mw::Record undo_record = table_manager.update(entry_id, s_income, s_income_description);
    table_manager.update(entry_id, s_expense, s_expense_description);
    int rows = countRows(records_table_name, accessor.getDbHandler());
    EXPECT_EQ(rows, 2 + 1);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 2 + 1);

    mw::Entry entry = table_manager.undo(entry_id);
    EXPECT_EQ(entry.getID(), entry_id);
    EXPECT_EQ(entry.getBalance(), s_entry_balance + s_income);
    EXPECT_EQ(entry.getBalance(), s_entry_balance + undo_record.getBalance());
    EXPECT_STREQ(entry.getDateTime().getDate().c_str(), undo_record.getDateTime().getDate().c_str());
    EXPECT_STREQ(entry.getDateTime().getTime().c_str(), undo_record.getDateTime().getTime().c_str());
    EXPECT_STREQ(entry.getDescription().c_str(), undo_record.getDescription().c_str());
    EXPECT_EQ(entry.getLastTransaction(), undo_record.getBalance());
    EXPECT_EQ(entry.getStatus(), undo_record.getStatus());
    rows = countRows(records_table_name, accessor.getDbHandler());
    EXPECT_EQ(rows, 1 + 1);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1 + 1);

    DB_Statement statement_handler = nullptr;
    std::string check_statement = "SELECT * FROM '";
    check_statement += records_table_name;
    check_statement += "';";
    int nByte = static_cast<int>(check_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_init = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_init, init.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id, undo_record.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    statement_handler = nullptr;
    check_statement = "SELECT * FROM '";
    check_statement += table_manager.getDailyTableName();
    check_statement += "';";
    nByte = static_cast<int>(check_statement.length());
    result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t record_id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(record_id, init.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    record_id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(record_id, undo_record.getID());
    std::string date(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1)));
    std::string time(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2)));
    mw::DateTime datetime(date, time);
    EXPECT_STREQ(undo_record.getDateTime().getDate().c_str(), datetime.getDate().c_str());
    EXPECT_STREQ(undo_record.getDateTime().getTime().c_str(), datetime.getTime().c_str());
    MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
    EXPECT_EQ(undo_record.getBalance(), balance);
    const void* raw_description = sqlite3_column_text(statement_handler, 4);
    mw::WrappedString description(static_cast<const wchar_t*>(raw_description));
    EXPECT_STREQ(undo_record.getDescription().c_str(), description.c_str());
    sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 5);
    EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::RecordStatusValue::RSV_INCOME));
    mw::RecordStatus status(raw_status);
    EXPECT_EQ(undo_record.getStatus(), status);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerUndoFreshEntry) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  MoneyValue_t s_entry_balance = 1000;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id = init.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

    std::string records_table_name = mw::TableManager::records_table_name_prefix + std::to_string(entry_id);
    int rows = countRows(records_table_name, accessor.getDbHandler());
    EXPECT_EQ(rows, 1);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1);

    mw::Entry entry = table_manager.undo(entry_id);
    EXPECT_EQ(entry.getID(), entry_id);
    EXPECT_EQ(entry.getBalance(), s_entry_balance);
    EXPECT_STREQ(entry.getDateTime().getDate().c_str(), init.first.getDateTime().getDate().c_str());
    EXPECT_STREQ(entry.getDateTime().getTime().c_str(), init.first.getDateTime().getTime().c_str());
    EXPECT_STREQ(entry.getDescription().c_str(), s_entry_description.c_str());
    EXPECT_EQ(entry.getLastTransaction(), 0);
    mw::RecordStatus init_status(mw::RecordStatusValue::RSV_UNKNOWN);
    EXPECT_EQ(entry.getStatus(), init_status);
    rows = countRows(records_table_name, accessor.getDbHandler());
    EXPECT_EQ(rows, 0 + 1);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0 + 1);

    DB_Statement statement_handler = nullptr;
    std::string check_statement = "SELECT * FROM '";
    check_statement += records_table_name;
    check_statement += "';";
    int nByte = static_cast<int>(check_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_init = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_init, init.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    statement_handler = nullptr;
    check_statement = "SELECT * FROM '";
    check_statement += table_manager.getDailyTableName();
    check_statement += "';";
    nByte = static_cast<int>(check_statement.length());
    result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t record_id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(record_id, init.second.getID());
    std::string date(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1)));
    std::string time(reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2)));
    mw::DateTime datetime(date, time);
    EXPECT_STREQ(entry.getDateTime().getDate().c_str(), datetime.getDate().c_str());
    MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
    EXPECT_EQ(entry.getBalance(), balance);
    const void* raw_description = sqlite3_column_text(statement_handler, 4);
    mw::WrappedString description(static_cast<const wchar_t*>(raw_description));
    EXPECT_STREQ(entry.getDescription().c_str(), description.c_str());
    sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 5);
    EXPECT_EQ(raw_status, static_cast<sqlite3_int64>(mw::RecordStatusValue::RSV_NEW_ENTRY));
    mw::RecordStatus status(mw::RecordStatusValue::RSV_UNKNOWN);
    EXPECT_EQ(entry.getStatus(), status);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (TableManagerTest, TableManagerUndoOnceUpdatedEntry) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_name = "Имя слота";
  mw::WrappedString s_entry_description = "Тестовое описание слота";
  mw::WrappedString s_income_description = "Доход в 1100 единиц";
  MoneyValue_t s_entry_balance = 1000;
  MoneyValue_t s_income = 1100;
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init = table_manager.add(s_name, s_entry_description, s_entry_balance);
    ID_t entry_id = init.first.getID();
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());

    std::string records_table_name = mw::TableManager::records_table_name_prefix + std::to_string(entry_id);
    mw::Record undo_record = table_manager.update(entry_id, s_income, s_income_description);
    int rows = countRows(records_table_name, accessor.getDbHandler());
    EXPECT_EQ(rows, 1 + 1);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 1 + 1);

    mw::Entry entry = table_manager.undo(entry_id);
    EXPECT_EQ(entry.getID(), entry_id);
    EXPECT_EQ(entry.getBalance(), s_entry_balance);
    EXPECT_STREQ(entry.getDateTime().getDate().c_str(), init.first.getDateTime().getDate().c_str());
    EXPECT_STREQ(entry.getDescription().c_str(), s_entry_description.c_str());
    EXPECT_EQ(entry.getLastTransaction(), 0);
    mw::RecordStatus init_status(mw::RecordStatusValue::RSV_UNKNOWN);
    EXPECT_EQ(entry.getStatus(), init_status);
    rows = countRows(records_table_name, accessor.getDbHandler());
    EXPECT_EQ(rows, 0 + 1);
    rows = countRows(table_manager.getDailyTableName(), accessor.getDbHandler());
    EXPECT_EQ(rows, 0 + 1);

    DB_Statement statement_handler = nullptr;
    std::string check_statement = "SELECT * FROM '";
    check_statement += records_table_name;
    check_statement += "';";
    int nByte = static_cast<int>(check_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_init = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_init, init.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);

    statement_handler = nullptr;
    check_statement = "SELECT * FROM '";
    check_statement += table_manager.getDailyTableName();
    check_statement += "';";
    nByte = static_cast<int>(check_statement.length());
    result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t record_id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(record_id, init.second.getID());
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (PolicyTableManagerTest, CreatePolicy) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_entry_1st_name = "Первый слот";
  mw::WrappedString s_entry_2nd_name = "Второй слот";
  mw::WrappedString s_entry_1st_description = "Тестовое описание первого слота";
  mw::WrappedString s_entry_2nd_description = "Тестовое описание второго слота";
  MoneyValue_t s_entry_1st_balance = 1750;
  MoneyValue_t s_entry_2nd_balance = 5250;
  mw::WrappedString s_policy_name = "Итог недели Корпорации";
  mw::WrappedString s_policy_description = "Передача средств корпорации в фонд";
  PolicyRatio_t s_ratio = 25;
  int s_hours_period = 168;
  mw::PolicyStatus s_policy_status(mw::PolicyStatusValue::PSV_ENABLED);
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init_1st = table_manager.add(s_entry_1st_name, s_entry_1st_description, s_entry_1st_balance);
    std::pair<mw::Entry, mw::Record> init_2nd = table_manager.add(s_entry_2nd_name, s_entry_2nd_description, s_entry_2nd_balance);
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::Policy policy = table_manager.createPolicy(s_policy_name, s_policy_description, s_ratio, init_1st.first.getID(), init_2nd.first.getID(), s_hours_period, s_policy_status);
    EXPECT_EQ(policy.getID(), 0);
    EXPECT_STREQ(policy.getName().c_str(), s_policy_name.c_str());
    EXPECT_STREQ(policy.getDescription().c_str(), s_policy_description.c_str());
    EXPECT_EQ(policy.getRatio(), s_ratio);
    EXPECT_EQ(policy.getSourceID(), init_1st.first.getID());
    EXPECT_EQ(policy.getDestinationID(), init_2nd.first.getID());
    EXPECT_EQ(policy.getPeriod(), s_hours_period);
    EXPECT_EQ(policy.getStatus(), s_policy_status);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (PolicyTableManagerTest, ApplyPolicy) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_entry_1st_name = "Первый слот";
  mw::WrappedString s_entry_2nd_name = "Второй слот";
  mw::WrappedString s_entry_1st_description = "Тестовое описание первого слота";
  mw::WrappedString s_entry_2nd_description = "Тестовое описание второго слота";
  MoneyValue_t s_entry_1st_balance = 1750;
  MoneyValue_t s_entry_2nd_balance = 5250;
  mw::WrappedString s_policy_name = "Итог недели Корпорации";
  mw::WrappedString s_policy_description = "Передача средств корпорации в фонд";
  PolicyRatio_t s_ratio = 25;
  int s_hours_period = 168;
  mw::PolicyStatus s_policy_status(mw::PolicyStatusValue::PSV_ENABLED);
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init_1st = table_manager.add(s_entry_1st_name, s_entry_1st_description, s_entry_1st_balance);
    std::pair<mw::Entry, mw::Record> init_2nd = table_manager.add(s_entry_2nd_name, s_entry_2nd_description, s_entry_2nd_balance);
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::Policy policy = table_manager.createPolicy(s_policy_name, s_policy_description, s_ratio, init_1st.first.getID(), init_2nd.first.getID(), s_hours_period, s_policy_status);
    EXPECT_TRUE(accessor.checkFinalized());

    mw::RecordStatus policy_status(mw::RecordStatusValue::RSV_APPLIED_POLICY);
    std::pair<mw::Record, mw::Record> records = table_manager.applyPolicy(policy.getID());
    MoneyValue_t value = mw::Policy::calculateRatioOfBalance(s_entry_1st_balance, s_ratio);
    EXPECT_EQ(records.first.getBalance(), -value);
    EXPECT_EQ(records.second.getBalance(), value);
    EXPECT_STREQ(records.first.getDescription().c_str(), policy.getDescription().c_str());
    EXPECT_STREQ(records.second.getDescription().c_str(), policy.getDescription().c_str());
    EXPECT_EQ(records.first.getStatus(), mw::RecordStatus(mw::RecordStatusValue::RSV_APPLIED_POLICY));
    EXPECT_EQ(records.second.getStatus(), mw::RecordStatus(mw::RecordStatusValue::RSV_APPLIED_POLICY));

    DB_Statement statement_handler = nullptr;
    std::string check_statement = "SELECT * FROM '";
    check_statement += table_manager.getCycleTableName();
    check_statement += "';";
    int nByte = static_cast<int>(check_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id, init_1st.first.getID());
    const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
    mw::WrappedString name(raw_name);
    EXPECT_STREQ(name.c_str(), init_1st.first.getName().c_str());
    const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
    mw::WrappedString description(raw_description);
    EXPECT_STRNE(description.c_str(), init_1st.first.getDescription().c_str());
    EXPECT_STREQ(description.c_str(), policy.getDescription().c_str());
    MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
    EXPECT_NE(balance, init_1st.first.getBalance());
    EXPECT_EQ(balance, s_entry_1st_balance - value);
    MoneyValue_t transaction = sqlite3_column_int64(statement_handler, 4);
    EXPECT_NE(transaction, init_1st.first.getLastTransaction());
    EXPECT_EQ(transaction, -value);
    sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 7);
    mw::RecordStatus status(raw_status);
    EXPECT_EQ(status, mw::RecordStatus(mw::RecordStatusValue::RSV_APPLIED_POLICY));

    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_dest = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_dest, init_2nd.first.getID());
    const void* raw_name_dest = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
    mw::WrappedString name_dest(raw_name_dest);
    EXPECT_STREQ(name_dest.c_str(), init_2nd.first.getName().c_str());
    const void* raw_description_dest = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
    mw::WrappedString description_dest(raw_description_dest);
    EXPECT_STRNE(description_dest.c_str(), init_2nd.first.getDescription().c_str());
    EXPECT_STREQ(description_dest.c_str(), policy.getDescription().c_str());
    MoneyValue_t balance_dest = sqlite3_column_int64(statement_handler, 3);
    EXPECT_NE(balance_dest, init_2nd.first.getBalance());
    EXPECT_EQ(balance_dest, s_entry_2nd_balance + value);
    MoneyValue_t transaction_dest = sqlite3_column_int64(statement_handler, 4);
    EXPECT_NE(transaction_dest, init_2nd.first.getLastTransaction());
    EXPECT_EQ(transaction_dest, value);
    sqlite3_int64 raw_status_dest = sqlite3_column_int64(statement_handler, 7);
    mw::RecordStatus status_dest(raw_status_dest);
    EXPECT_EQ(status_dest, mw::RecordStatus(mw::RecordStatusValue::RSV_APPLIED_POLICY));
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (PolicyTableManagerTest, ApplyPolicyAndUndo) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_entry_1st_name = "Первый слот";
  mw::WrappedString s_entry_2nd_name = "Второй слот";
  mw::WrappedString s_entry_1st_description = "Тестовое описание первого слота";
  mw::WrappedString s_entry_2nd_description = "Тестовое описание второго слота";
  MoneyValue_t s_entry_1st_balance = 1750;
  MoneyValue_t s_entry_2nd_balance = 5250;
  mw::WrappedString s_policy_name = "Итог недели Корпорации";
  mw::WrappedString s_policy_description = "Передача средств корпорации в фонд";
  PolicyRatio_t s_ratio = 25;
  int s_hours_period = 168;
  mw::PolicyStatus s_policy_status(mw::PolicyStatusValue::PSV_ENABLED);
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    std::pair<mw::Entry, mw::Record> init_1st = table_manager.add(s_entry_1st_name, s_entry_1st_description, s_entry_1st_balance);
    std::pair<mw::Entry, mw::Record> init_2nd = table_manager.add(s_entry_2nd_name, s_entry_2nd_description, s_entry_2nd_balance);
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());
    mw::Policy policy = table_manager.createPolicy(s_policy_name, s_policy_description, s_ratio, init_1st.first.getID(), init_2nd.first.getID(), s_hours_period, s_policy_status);
    EXPECT_TRUE(accessor.checkFinalized());

    mw::RecordStatus policy_status(mw::RecordStatusValue::RSV_APPLIED_POLICY);
    std::pair<mw::Record, mw::Record> records = table_manager.applyPolicy(policy.getID());
    MoneyValue_t value = mw::Policy::calculateRatioOfBalance(s_entry_1st_balance, s_ratio);

    mw::Entry undo_entry_1st = table_manager.undo(init_1st.first.getID());
    EXPECT_EQ(undo_entry_1st.getID(), init_1st.first.getID());
    EXPECT_STREQ(undo_entry_1st.getName().c_str(), init_1st.first.getName().c_str());
    EXPECT_STREQ(undo_entry_1st.getDescription().c_str(), init_1st.first.getDescription().c_str());
    EXPECT_EQ(undo_entry_1st.getBalance(), init_1st.first.getBalance());
    EXPECT_EQ(undo_entry_1st.getLastTransaction(), init_1st.first.getLastTransaction());
    EXPECT_EQ(undo_entry_1st.getStatus(), init_1st.first.getStatus());

    mw::Entry undo_entry_2nd = table_manager.undo(init_2nd.first.getID());
    EXPECT_EQ(undo_entry_2nd.getID(), init_2nd.first.getID());
    EXPECT_STREQ(undo_entry_2nd.getName().c_str(), init_2nd.first.getName().c_str());
    EXPECT_STREQ(undo_entry_2nd.getDescription().c_str(), init_2nd.first.getDescription().c_str());
    EXPECT_EQ(undo_entry_2nd.getBalance(), init_2nd.first.getBalance());
    EXPECT_EQ(undo_entry_2nd.getLastTransaction(), init_2nd.first.getLastTransaction());
    EXPECT_EQ(undo_entry_2nd.getStatus(), init_2nd.first.getStatus());

    DB_Statement statement_handler = nullptr;
    std::string check_statement = "SELECT * FROM '";
    check_statement += table_manager.getCycleTableName();
    check_statement += "';";
    int nByte = static_cast<int>(check_statement.length());
    int result = sqlite3_prepare_v2(
        accessor.getDbHandler(),
        check_statement.c_str(),
        nByte,
        &statement_handler,
        nullptr);
    EXPECT_TRUE(statement_handler);
    EXPECT_EQ(result, SQLITE_OK);
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id, init_1st.first.getID());
    const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
    mw::WrappedString name(raw_name);
    EXPECT_STREQ(name.c_str(), init_1st.first.getName().c_str());
    const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
    mw::WrappedString description(raw_description);
    EXPECT_STREQ(description.c_str(), init_1st.first.getDescription().c_str());
    EXPECT_STRNE(description.c_str(), policy.getDescription().c_str());
    MoneyValue_t balance = sqlite3_column_int64(statement_handler, 3);
    EXPECT_EQ(balance, init_1st.first.getBalance());
    EXPECT_NE(balance, s_entry_1st_balance - value);
    MoneyValue_t transaction = sqlite3_column_int64(statement_handler, 4);
    EXPECT_EQ(transaction, init_1st.first.getLastTransaction());
    EXPECT_NE(transaction, -value);
    sqlite3_int64 raw_status = sqlite3_column_int64(statement_handler, 7);
    mw::RecordStatus status(raw_status);
    EXPECT_EQ(status, mw::RecordStatus(mw::RecordStatusValue::RSV_UNKNOWN));

    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_ROW);
    ID_t id_dest = sqlite3_column_int64(statement_handler, 0);
    EXPECT_EQ(id_dest, init_2nd.first.getID());
    const void* raw_name_dest = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 1));
    mw::WrappedString name_dest(raw_name_dest);
    EXPECT_STREQ(name_dest.c_str(), init_2nd.first.getName().c_str());
    const void* raw_description_dest = reinterpret_cast<const char*>(sqlite3_column_text(statement_handler, 2));
    mw::WrappedString description_dest(raw_description_dest);
    EXPECT_STREQ(description_dest.c_str(), init_2nd.first.getDescription().c_str());
    EXPECT_STRNE(description_dest.c_str(), policy.getDescription().c_str());
    MoneyValue_t balance_dest = sqlite3_column_int64(statement_handler, 3);
    EXPECT_EQ(balance_dest, init_2nd.first.getBalance());
    EXPECT_NE(balance_dest, s_entry_2nd_balance + value);
    MoneyValue_t transaction_dest = sqlite3_column_int64(statement_handler, 4);
    EXPECT_EQ(transaction_dest, init_2nd.first.getLastTransaction());
    EXPECT_NE(transaction_dest, value);
    sqlite3_int64 raw_status_dest = sqlite3_column_int64(statement_handler, 7);
    mw::RecordStatus status_dest(raw_status_dest);
    EXPECT_EQ(status_dest, mw::RecordStatus(mw::RecordStatusValue::RSV_UNKNOWN));
    result = sqlite3_step(statement_handler);
    EXPECT_EQ(result, SQLITE_DONE);
    sqlite3_finalize(statement_handler);
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}

TEST (PolicyTableManagerTest, UndoPolicy) {
  // TODO: impl
}

/*TEST (PolicyTableManagerTest, DeletePolicy) {
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  mw::WrappedString s_entry_1st_name = "Первый слот";
  mw::WrappedString s_entry_2nd_name = "Второй слот";
  mw::WrappedString s_entry_1st_description = "Тестовое описание первого слота";
  mw::WrappedString s_entry_2nd_description = "Тестовое описание второго слота";
  MoneyValue_t s_entry_1st_balance = 1750;
  MoneyValue_t s_entry_2nd_balance = 5250;
  mw::WrappedString s_policy_name = "Итог недели Корпорации";
  mw::WrappedString s_policy_description = "Передача средств корпорации в фонд";
  PolicyRatio_t s_ratio = 25;
  int s_hours_period = 168;
  mw::PolicyStatus s_policy_status(mw::PolicyStatusValue::PSV_ENABLED);
  try {
    mw::TableManager table_manager;
    EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 1);
    EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 1);
    EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 1);
    EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 1);
    mw::TestAccessTable<mw::TableManager> accessor(&table_manager);
    EXPECT_TRUE(accessor.checkFinalized());
    // TODO: impl
    EXPECT_TRUE(accessor.checkFinalized());

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
  EXPECT_EQ(mw::TableManager::OPENED_DATABASES_COUNT, 0);
  EXPECT_EQ(mw::CycleTable::OPENED_CYCLE_TABLES_COUNT, 0);
  EXPECT_EQ(mw::DailyTable::OPENED_DAILY_TABLES_COUNT, 0);
  EXPECT_EQ(mw::PolicyTable::OPENED_POLICY_TABLES_COUNT, 0);
  remove(mw::TableManager::single_database_name.c_str());
}*/

#endif  // ENABLED_TIME_MEASURE_ONLY


/* Main */
// ----------------------------------------------------------------------------
int main(int argc, char** argv) {
  //setlocale(LC_CTYPE, "");
  ::testing::InitGoogleTest(&argc, argv);
  return (RUN_ALL_TESTS());
}
