/*
 * cycle_table.cc
 *
 *  Description: Cycle Table class implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <algorithm>
#include <utility>
#include "cycle_table.h"
#include "logger.h"
#include "service.h"
#include "sqlite3.h"


namespace mw {

int CycleTable::OPENED_CYCLE_TABLES_COUNT = 0;
const std::string CycleTable::last_row_id_table_name = "Last_Entry_ID";

CycleTable::CycleTable(const std::string& i_db_name)
  : iDatabase(i_db_name, "Cycle_Table") {
  INF("enter CycleTable constructor.");
  this->__init__();
  ++CycleTable::OPENED_CYCLE_TABLES_COUNT;
  INF("exit CycleTable constructor.");
}

CycleTable::CycleTable(CycleTable&& rval_obj)
  : iDatabase(std::move(static_cast<iDatabase&>(rval_obj))) {
}

CycleTable::~CycleTable() {
  INF("enter CycleTable destructor.");
  this->__close_database__();
  --CycleTable::OPENED_CYCLE_TABLES_COUNT;
  INF("exit CycleTable destructor.");
}

Entry CycleTable::addEntry(
    const WrappedString& i_name,
    const WrappedString& i_description,
    const MoneyValue_t& i_current_balance) {
  INF("enter CycleTable::addEntry().");
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_table_name;
  insert_statement += "' VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8);";
  this->__prepare_statement__(insert_statement);

  bool accumulate = true;
  ID_t entry_id = this->m_next_id++;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, entry_id) == SQLITE_OK);
  DBG("ID [%lli] has been stored in table ["%s"], SQLite database ["%s"].",
      entry_id, this->m_table_name.c_str(), this->m_db_name.c_str());

  int name_n_bytes = i_name.n_bytes();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          2,
          i_name.c_str(),
          name_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG1("Name ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
       i_name.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  int description_n_bytes = i_description.n_bytes();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          3,
          i_description.c_str(),
          description_n_bytes,
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG2("Description ["%s"] has been stored in table ["%s"], "
      "SQLite database ["%s"].",
       i_description.c_str(),
       this->m_table_name.c_str(),
       this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          4,
          i_current_balance) == SQLITE_OK);
  DBG("Current balance [%lli] has been stored in table ["%s"], "
      "SQLite database ["%s"].",
      i_current_balance, this->m_table_name.c_str(), this->m_db_name.c_str());

  MoneyValue_t last_transaction = 0;
  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          5,
          last_transaction) == SQLITE_OK);
  DBG1("Last transaction balance [%lli] has been stored in table ["%s"], "
      "SQLite database ["%s"].",
       last_transaction, this->m_table_name.c_str(), this->m_db_name.c_str());

  DateTime current_datetime;
  std::string date = current_datetime.getDate();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          6,
          date.c_str(),
          date.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG2("Date ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
       date.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          7,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Time ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
      time.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  Status status = SV_UNKNOWN;
  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          8,
          static_cast<sqlite3_int64>(status)) == SQLITE_OK);
  DBG1("Status [%lli] has been stored in table ["%s"], "
      "SQLite database ["%s"].",
       static_cast<sqlite3_int64>(status),
       this->m_table_name.c_str(),
       this->m_db_name.c_str());

  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into table ["%s"], database ["%s"] "
        "by statement ["%s"]!",
        this->m_table_name.c_str(),
        this->m_db_name.c_str(),
        insert_statement.c_str());
    this->__finalize_and_throw__(
        insert_statement.c_str(),
        SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG2("All insertions have succeeded.");
  }

#if ENABLED_DB_CACHING
  // TODO: caching the entry
#endif

  this->__finalize__(insert_statement.c_str());
  this->__increment_rows__();
  this->__write_last_id__(CycleTable::last_row_id_table_name, entry_id);
  Entry entry(
      entry_id,
      i_name,
      i_description,
      i_current_balance,
      last_transaction,
      status,
      current_datetime);
  DBG("Constructed output entry.");

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit CycleTable::addEntry().");
  return (entry);
}

Entry CycleTable::readEntry(const ID_t& i_entry_id) {
  INF("enter CycleTable::readEntry().");

#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_entry_id);
#endif

  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE ID == '";
  select_statement += std::to_string(i_entry_id);
  select_statement += "';";
  this->__prepare_statement__(select_statement);
  sqlite3_step(this->m_db_statement);
  ID_t id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG("Read id [%lli] from  table ["%s"] of database ["%s"], "
      "input id was [%lli].",
      id, this->m_table_name.c_str(), this->m_db_name.c_str(), i_entry_id);
  TABLE_ASSERT("Input entry id does not equal to primary key value "
               "from database!" &&
               id == i_entry_id);

  const void* raw_name = reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 1));
  WrappedString name(raw_name);
  const void* raw_description = reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 2));
  WrappedString description(raw_description);
  MoneyValue_t balance = sqlite3_column_int64(this->m_db_statement, 3);
  MoneyValue_t transaction = sqlite3_column_int64(this->m_db_statement, 4);
  std::string date(reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 5)));
  std::string time(reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 6)));
  DateTime datetime(date, time);
  sqlite3_int64 raw_status = sqlite3_column_int64(this->m_db_statement, 7);
  Status status(raw_status);
  DBG1("Loaded column data: Name ["%s"]; Description ["%s"]; Balance [%lli]; "
       "Transaction [%lli]; Date ["%s"]; Time ["%s"]; Status [%lli].",
       name.c_str(),
       description.c_str(),
       balance,
       transaction,
       datetime.getDate().c_str(),
       datetime.getTime().c_str(),
       raw_status);
  Entry entry(id, name, description, balance, transaction, status, datetime);
  DBG2("Proper entry instance has been constructed.");

#if ENABLED_DB_CACHING
  // TODO: caching the entry
#endif

  this->__finalize__(select_statement.c_str());
  INF("exit CycleTable::readEntry().");
  return (entry);
}

Entry CycleTable::updateEntry(
    const ID_t& i_entry_id,
    const MoneyValue_t& i_value,
    const WrappedString& i_description) {
  INF("enter CycleTable::updateEntry().");

#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_entry_id);
#endif

  Entry entry = this->readEntry(i_entry_id);
  DBG("Got entry from SQLite database.");
  entry.updateBalance(i_value, i_description);
  DBG1("Updated entry [ID: %lli].", i_entry_id);
  WrappedString update_statement = "UPDATE '";
  update_statement += WrappedString(this->m_table_name);
  update_statement += "' SET Description = '";
  update_statement += i_description;
  update_statement += "', CurrentBalance = '";
  update_statement += WrappedString::to_string(entry.getBalance());
  update_statement += "', LastTransaction = '";
  update_statement += WrappedString::to_string(i_value);
  update_statement += "', Date = '";
  update_statement += WrappedString(entry.getDateTime().getDate());
  update_statement += "', Time = '";
  update_statement += WrappedString(entry.getDateTime().getTime());
  update_statement += "', Status = '";
  update_statement +=
      WrappedString::to_string(static_cast<sqlite3_int64>(entry.getStatus()));
  update_statement += "' WHERE ID == '";
  update_statement += WrappedString::to_string(i_entry_id);
  update_statement += "';";
  this->__prepare_statement__(update_statement);
  sqlite3_step(this->m_db_statement);

#if ENABLED_DB_CACHING
  // TODO: caching the entry
#endif

  this->__finalize__(update_statement.c_str());
  DBG2("Updated database row for entry [ID: %lli] in table ["%s"].",
       i_entry_id, this->m_table_name.c_str());
  INF("exit CycleTable::updateEntry().");
  return (entry);
}

Entry CycleTable::rollbackEntry(
    const ID_t& i_entry_id,
    const MoneyValue_t& i_value,
    const Record& i_record) {
  INF("enter CycleTable::rollbackEntry().");

#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_entry_id);
#endif

  Entry entry = this->readEntry(i_entry_id);
  DBG("Got entry from SQLite database.");
  entry.rollbackBalance(i_value, i_record);
  DBG1("Rolled back entry [ID: %lli].", i_entry_id);
  WrappedString update_statement = "UPDATE '";
  update_statement += WrappedString(this->m_table_name);
  update_statement += "' SET Description = '";
  update_statement += entry.getDescription();
  update_statement += "', CurrentBalance = '";
  update_statement += WrappedString::to_string(entry.getBalance());
  update_statement += "', LastTransaction = '";
  update_statement += WrappedString::to_string(entry.getLastTransaction());
  update_statement += "', Date = '";
  update_statement += WrappedString(entry.getDateTime().getDate());
  update_statement += "', Time = '";
  update_statement += WrappedString(entry.getDateTime().getTime());
  update_statement += "', Status = '";
  update_statement +=
      WrappedString::to_string(static_cast<sqlite3_int64>(entry.getStatus()));
  update_statement += "' WHERE ID == '";
  update_statement += WrappedString::to_string(i_entry_id);
  update_statement += "';";
  this->__prepare_statement__(update_statement);
  sqlite3_step(this->m_db_statement);

#if ENABLED_DB_CACHING
  // TODO: caching the entry
#endif

  this->__finalize__(update_statement.c_str());
  DBG2("Updated database row for entry [ID: %lli] in table ["%s"].",
       i_entry_id, this->m_table_name.c_str());
  INF("exit CycleTable::rollbackEntry().");
  return (entry);
}

void CycleTable::deleteEntry(const ID_t& i_entry_id) {
  INF("enter CycleTable::deleteEntry().");

#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_entry_id);
#endif

  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID == '";
  delete_statement += std::to_string(i_entry_id);
  delete_statement += "';";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  this->__decrement_rows__();
  if (i_entry_id + 1 == this->m_next_id) {
    --this->m_next_id;
    DBG2("Deleted entry with largest ID. Next ID value has been decremented.");
  }
  if (this->__empty__()) {
    DBG("Table ["%s"] has become empty. Next ID value is set to zero.",
        this->m_table_name.c_str());
    this->m_next_id = 0;
  }
  DBG1("Deleted entry [ID: %lli] in table ["%s"].",
       i_entry_id, this->m_table_name.c_str());

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit CycleTable::deleteEntry().");
}

void CycleTable::deleteEntries(std::vector<ID_t>& i_entry_ids) {
  INF("enter CycleTable::deleteRecords().");
  std::sort(i_entry_ids.begin(), i_entry_ids.end());

#if ENABLED_ADVANCED_DEBUG
  for (ID_t& entry_id : i_entry_ids) {
    this->__where_check__(entry_id);
  }
#endif

  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID IN(";
  delete_statement += vectorToString(i_entry_ids, ", ");
  delete_statement += ");";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  this->__decrease_rows__(static_cast<int>(i_entry_ids.size()));
  DBG("Updating next ID if necessary, initial value is [%lli].",
      this->m_next_id);
  for (std::vector<ID_t>::const_reverse_iterator it = i_entry_ids.rbegin();
       it != i_entry_ids.rend();
       ++it) {
    if (*it + 1 == this->m_next_id) {
      --this->m_next_id;
      DBG2("Deleted entry with largest ID. "
           "Next ID value has been decremented.");
    } else {
      break;
    }
  }
  DBG("Finished updating next ID: [%lli].",
      this->m_next_id);
  if (this->__empty__()) {
    DBG("Table ["%s"] has become empty. Next ID value is set to zero.",
        this->m_table_name.c_str());
    this->m_next_id = 0;
  }
  DBG1("Deleted %lli entries from table ["%s"].",
       static_cast<long long int>(i_entry_ids.size()),
       this->m_table_name.c_str());

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit CycleTable::deleteRecords().");
}

const std::string& CycleTable::getName() const {
  INF("enter CycleTable::getName().");
  DBG("Return the name ["%s"] of CycleTable at %p.",
      this->m_table_name.c_str(), this);
  INF("exit CycleTable::getName().");
  return (this->m_table_name);
}


/* Private members */
// ----------------------------------------------------------------------------
void CycleTable::__init__() {
  DBG("enter CycleTable::__init__().");
  iDatabase::__init__();
  iDatabase::__create_table_for_last_id__(CycleTable::last_row_id_table_name);
  ID_t last_row_id = this->__read_last_id__(CycleTable::last_row_id_table_name);
  this->m_next_id = last_row_id == 0 ? 0 : last_row_id + 1;
  TRC("Initialization has completed: total rows [%i], last row id [%lli], "
      "next_id [%lli].",
      this->m_rows, last_row_id, this->m_next_id);
  DBG("exit CycleTable::__init__().");
}

void CycleTable::__create_table__() {
  DBG("enter CycleTable::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += this->m_table_name;
  statement += "('ID' INTEGER PRIMARY KEY, "
      "'Name' TEXT, "
      "'Description' TEXT, "
      "'CurrentBalance' INTEGER, "
      "'LastTransaction' INTEGER, "
      "'Date' TEXT, "
      "'Time' TEXT, "
      "'Status' INTEGER);";
  this->__prepare_statement__(statement);
  sqlite3_step(this->m_db_statement);
  DBG1("Table ["%s"] has been successfully created.",
       this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG("exit CycleTable::__create_table__().");
}

}  /* namespace mw */
