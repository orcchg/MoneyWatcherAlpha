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
#include <vector>
#include "tests/google/gtest/gtest.h"
#include "cycle_table.h"
#include "daily_table.h"
#include "table_manager.h"


#define FIXTURE_TOTAL_DB_ROWS 3

/* Tables time measurement fixture */
// ----------------------------------------------------------------------------
class TimeMeasureFixture : public ::testing::Test {
public:
  TimeMeasureFixture()
    : m_cycle_table(TimeMeasureFixture::test_db_filename)
    , m_daily_table(TimeMeasureFixture::test_db_filename) {
      // initialization code here
    entry_ids.reserve(TimeMeasureFixture::total_rows);
    record_ids.reserve(TimeMeasureFixture::total_rows);
  }

  ~TimeMeasureFixture() {
      // cleanup any pending stuff, but no exceptions allowed
  }

  void SetUp() {
      // code here will execute just before the test ensues
    mw::WrappedString s_name = "Имя слота";
    MoneyValue_t s_balance = 1000;
    mw::WrappedString s_description = "Тестовая запись в таблице";
    mw::Status s_status(mw::SV_INCOME);

    for (int index = 0; index < TimeMeasureFixture::total_rows; ++index) {
      mw::Entry entry = this->m_cycle_table.addEntry(s_name, s_description, s_balance);
      this->entry_ids.push_back(entry.getID());
      mw::Record record = this->m_daily_table.addRecord(s_balance, s_description, s_status);
      this->record_ids.push_back(record.getID());
    }
  }

  void TearDown() {
      // code here will be called just after the test completes
      // ok to through exceptions from here if need be
    remove(test_db_filename.c_str());
  }

protected:
  // put in any custom data members that you need
  static const int total_rows = FIXTURE_TOTAL_DB_ROWS;
  static const std::string test_db_filename;

  mw::CycleTable& getCycleTable() { return (this->m_cycle_table); }
  mw::DailyTable& getDailyTable() { return (this->m_daily_table); }
  const std::vector<ID_t>& getEntryIDs() const { return (this->entry_ids); }
  const std::vector<ID_t>& getRecordIDs() const { return (this->record_ids); }

private:
  mw::CycleTable m_cycle_table;
  mw::DailyTable m_daily_table;

  std::vector<ID_t> entry_ids;
  std::vector<ID_t> record_ids;
};


/* TableManager time measurement fixture */
// ----------------------------------------------------------------------------
class TableManagerTimeMeasureFixture : public ::testing::Test {
public:
  TableManagerTimeMeasureFixture()
    : m_table_manager(mw::TableManager()) {
  }

  ~TableManagerTimeMeasureFixture() {
      // cleanup any pending stuff, but no exceptions allowed
  }

  void SetUp() {
      // code here will execute just before the test ensues
    mw::WrappedString s_name = "Имя слота";
    MoneyValue_t s_balance = 1000;
    mw::WrappedString s_description = "Тестовая запись в таблице";

    for (int index = 0; index < TableManagerTimeMeasureFixture::total_rows; ++index) {
      ID_t id = this->m_table_manager.add(s_name, s_description, s_balance);
      this->entry_ids.push_back(id);
    }
  }

  void TearDown() {
      // code here will be called just after the test completes
      // ok to through exceptions from here if need be
    remove(mw::TableManager::single_database_name.c_str());
  }

protected:
  // put in any custom data members that you need
  static const int total_rows = FIXTURE_TOTAL_DB_ROWS;

  mw::TableManager& getTableManager() { return (this->m_table_manager); }
  const std::vector<ID_t>& getEntryIDs() const { return (this->entry_ids); }

private:
  mw::TableManager m_table_manager;
  std::vector<ID_t> entry_ids;
};


#endif /* TEST_FIXTURE_H_ */
