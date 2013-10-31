/*
 * table_manager.cc
 *
 *  Description: TableManager class implementation.
 *
 *  Created on: 12.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <string>
#include <utility>
#include <vector>
#include "logger.h"
#include "table_manager.h"


namespace mw {

int TableManager::OPENED_DATABASES_COUNT = 0;
const std::string TableManager::single_database_name = "MW-Database.db";
const std::string TableManager::records_table_name_prefix = "Records_for_Entry_";

TableManager::TableManager()
  : iDatabase(TableManager::single_database_name, "Entry_IDs_Table")
  , m_cycle_table(TableManager::single_database_name)
  , m_daily_table(TableManager::single_database_name)
  , m_policy_table(TableManager::single_database_name) {
  INF("enter TableManager constructor.");
  this->__init__();
  ++TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager constructor.");
}

TableManager::TableManager(TableManager&& rval_obj)
  : iDatabase(std::move(static_cast<iDatabase&>(rval_obj))) {
}

TableManager::~TableManager() {
  INF("enter TableManager destructor.");
  this->__close_database__();
  --TableManager::OPENED_DATABASES_COUNT;
  INF("exit TableManager destructor.");
}

// ----------------------------------------------------------------------------
std::pair<Entry, Record> TableManager::add(
    const WrappedString& i_name,
    const WrappedString& i_description,
    const MoneyValue_t& i_current_balance) {
  INF("enter TableManager::add().");
  Entry entry =
      this->m_cycle_table.addEntry(
          i_name,
          i_description,
          i_current_balance);
  ID_t entry_id = entry.getID();
  this->m_entry_ids.insert(entry_id);
  DBG3("Added entry into table ["%s"], got ID: [%lli].",
       this->getCycleTableName().c_str(), entry_id);
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_table_name;
  insert_statement += "' VALUES(?1, ?2);";
  this->__prepare_statement__(insert_statement);

  bool accumulate = true;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, entry_id) == SQLITE_OK);
  DBG3("ID [%lli] has been stored in SQLite database ["%s"].",
       entry_id, this->m_db_name.c_str());

  std::string records_table_name =
      TableManager::records_table_name_prefix + std::to_string(entry_id);
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          2,
          records_table_name.c_str(),
          records_table_name.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG3("Records table name ["%s"] has been stored in SQLite database ["%s"].",
       records_table_name.c_str(), this->m_db_name.c_str());

  sqlite3_step(this->m_db_statement);
  if (!accumulate) {
    ERR("Error during saving data into database ["%s"] by statement ["%s"]!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(
        insert_statement.c_str(),
        SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG3("All insertions have succeeded.");
  }
  this->__finalize__(insert_statement.c_str());
  this->__create_table_entry_records__(records_table_name);
  DBG3("Created table with name ["%s"] for records of entry [ID: %lli].",
       records_table_name.c_str(), entry_id);

  // add record, storing entry's initial state values
  Record record =
      this->m_daily_table.addRecord(
          i_current_balance,
          i_description,
          entry.getStatus());
  ID_t record_id = record.getID();
  DBG3("Added record into table ["%s"], got ID: [%lli].",
       this->getDailyTableName().c_str(), record_id);
  insert_statement = "INSERT INTO '";
  insert_statement += records_table_name;
  insert_statement += "' VALUES(?1);";
  this->__prepare_statement__(insert_statement);
  accumulate = sqlite3_bind_int64(this->m_db_statement, 1, record_id);
  sqlite3_step(this->m_db_statement);
  if (accumulate != SQLITE_OK) {
    ERR("Error during saving data into database ["%s"] by statement ["%s"]!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(
        insert_statement.c_str(),
        SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG3("All insertions have succeeded.");
  }
  this->__finalize__(insert_statement.c_str());

#if ENABLED_DB_CACHING
  // TODO: caching the entry's records
#endif

  INF("exit TableManager::add().");
  return (std::pair<Entry, Record>(entry, record));
}

Record TableManager::update(
    const ID_t& i_entry_id,
    const MoneyValue_t& i_value,
    const WrappedString& i_description){
  INF("enter TableManager::update().");
  Entry entry =
      this->m_cycle_table.updateEntry(
          i_entry_id,
          i_value,
          i_description);
  Record record =
      this->m_daily_table.addRecord(
          i_value,
          i_description,
          entry.getStatus());
  ID_t record_id = record.getID();

  DBG3("Reading table name of entry records from SQLite database ["%s"]...",
       this->m_db_name.c_str());
  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE EntryID == '";
  select_statement += std::to_string(i_entry_id);
  select_statement += "';";
  this->__prepare_statement__(select_statement);
  sqlite3_step(this->m_db_statement);
  std::string records_table_name(reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 1)));
  DBG3("Got record [ID: %lli], inserting into table ["%s"] "
       "of entry [ID: %lli].",
       record_id, records_table_name.c_str(), entry.getID());
  this->__finalize__(select_statement.c_str());

  std::string insert_statement = "INSERT INTO '";
  insert_statement += records_table_name;
  insert_statement += "' VALUES(?1);";
  this->__prepare_statement__(insert_statement);
  int accumulate = sqlite3_bind_int64(this->m_db_statement, 1, record_id);
  sqlite3_step(this->m_db_statement);
  if (accumulate != SQLITE_OK) {
    ERR("Error during saving data into database ["%s"] by statement ["%s"]!",
        this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(
        insert_statement.c_str(),
        SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG3("All insertions have succeeded.");
  }
  this->__finalize__(insert_statement.c_str());
  INF("exit TableManager::update().");
  return (record);
}

void TableManager::remove(const ID_t& i_entry_id) {
  INF("enter TableManager::remove().");
  this->m_cycle_table.deleteEntry(i_entry_id);
  this->m_entry_ids.erase(i_entry_id);
  DBG3("Deleted entry [ID: %lli] from Cycle_Table at %p.",
       i_entry_id, &(this->m_cycle_table));
  std::string records_table_name =
      TableManager::records_table_name_prefix + std::to_string(i_entry_id);
  std::string select_statement = "SELECT * FROM '";
  select_statement += records_table_name;
  select_statement += "' ORDER BY RecordID ASC;";
  int result = this->__prepare_statement__(select_statement);
  DBG3("Loop deletion of records by its ID...");
  std::vector<ID_t> record_ids;
  record_ids.reserve(10000);
  result = sqlite3_step(this->m_db_statement);
  while (result == SQLITE_ROW) {
    ID_t record_id = sqlite3_column_int64(this->m_db_statement, 0);
    record_ids.push_back(record_id);
    DBG3("Got record [ID: %lli] from table ["%s"].",
         record_id, records_table_name.c_str());
    result = sqlite3_step(this->m_db_statement);
  }
  this->__finalize__(select_statement.c_str());
  // Unable to perform record deletion in-place with record ID reading
  // from another table, because when certain sqlite_step() is in progress,
  // other tables become READ ONLY, so 'INSERT', 'UPDATE' and 'DELETE'
  // are prohibited.
#if ENABLED_ADVANCED_DEBUG
  for (ID_t& record_id : record_ids) {
    this->m_daily_table.deleteRecord(record_id);
    DBG3("Deleted record [ID: %lli] from table ["%s"].",
         record_id, this->m_daily_table.getName().c_str());
  }
#else
  // this approach is a bit faster, but less informative, then the above
  this->m_daily_table.deleteRecords(record_ids);
#endif
  DBG3("Deleted all records corresponding to entry [ID: %lli].",
       i_entry_id);

  this->__drop_table__(records_table_name);
  DBG3("Table with records ["%s"] has been dropped.",
       records_table_name.c_str());

  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE EntryID == '";
  delete_statement += std::to_string(i_entry_id);
  delete_statement += "';";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  DBG3("Removed row of entry [ID: %lli] from table ["%s"].",
       i_entry_id, this->m_table_name.c_str());
  INF("exit TableManager::remove().");
}

Entry TableManager::undo(const ID_t& i_entry_id) {
  INF("enter TableManager::undo().");
  std::string records_table_name =
      TableManager::records_table_name_prefix + std::to_string(i_entry_id);
  std::string select_statement = "SELECT * FROM '";
  select_statement += records_table_name;
  select_statement += "' ORDER BY RecordID DESC LIMIT 2;";
  int result = this->__prepare_statement__(select_statement);
  sqlite3_step(this->m_db_statement);
  ID_t last_record_id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG3("Got last record [ID: %lli] from table ["%s"]. To be deleted...",
       last_record_id, records_table_name.c_str());
  result = sqlite3_step(this->m_db_statement);
  if (result == SQLITE_DONE) {
    DBG3("Table ["%s"] is empty, nothing to be done with entry [ID: %lli].",
         records_table_name.c_str(), i_entry_id);
    this->__finalize__(select_statement.c_str());
    Entry entry = this->m_cycle_table.readEntry(i_entry_id);
    INF("exit TableManager::undo().");
    return (entry);
  }
  ID_t undo_record_id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG3("Got undo record [ID: %lli] from table ["%s"]. "
       "Entry [ID: %lli] will be rolled back...",
       undo_record_id, records_table_name.c_str(), i_entry_id);
  this->__finalize__(select_statement.c_str());

  Record last_record = this->m_daily_table.readRecord(last_record_id);
  this->m_daily_table.deleteRecord(last_record_id);
  DBG3("Deleted last record [ID: %lli] from table ["%s"].",
       last_record_id, this->m_daily_table.getName().c_str());

  std::string delete_statement = "DELETE FROM '";
  delete_statement += records_table_name;
  delete_statement += "' WHERE RecordID == '";
  delete_statement += std::to_string(last_record_id);
  delete_statement += "';";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());

  Record undo_record = this->m_daily_table.readRecord(undo_record_id);
  Entry entry =
      this->m_cycle_table.rollbackEntry(
          i_entry_id,
          last_record.getBalance(),
          undo_record);
  DBG3("Entry [ID: %lli] has been rolled back to state "
       "it had on ["%s"] at ["%s"].",
       i_entry_id,
       undo_record.getDateTime().getDate().c_str(),
       undo_record.getDateTime().getTime().c_str());
  INF("exit TableManager::undo().");
  return (entry);
}

// ----------------------------------------------------------------------------
Policy TableManager::createPolicy(
      const WrappedString& i_name,
      const WrappedString& i_description,
      const PolicyRatio_t& i_ratio,
      const ID_t& i_source_entry_id,
      const ID_t& i_destination_entry_id,
      int i_hours_period,
      const PolicyStatus& i_status) {
  INF("enter TableManager::createPolicy().");
  Policy policy = this->m_policy_table.addPolicy(
      i_name,
      i_description,
      i_ratio,
      i_source_entry_id,
      i_destination_entry_id,
      i_hours_period,
      i_status);
  DBG3("Created policy: ID [%lli]; Name ["%s"]; Description ["%s"]; "
       "Ratio [%lli]; Source entry ID [%lli], Destination entry ID [%lli]; "
       "Period [%i hours]; Status [%lli].",
       policy.getID(),
       policy.getName().c_str(),
       policy.getDescription().c_str(),
       policy.getRatio(),
       policy.getSourceID(),
       policy.getDestinationID(),
       policy.getPeriod(),
       static_cast<sqlite3_int64>(policy.getStatus()));
  INF("exit TableManager::createPolicy().");
  return (policy);
}

Record TableManager::applyPolicy(const ID_t& i_policy_id) {
  INF("enter TableManager::applyPolicy().");
  std::shared_ptr<DateTime> ptr_datetime;
  Policy policy = this->m_policy_table.readPolicy(i_policy_id, ptr_datetime);
  DBG3("Read policy: ID [%lli]; Name ["%s"]; Description ["%s"]; "
       "Ratio [%lli]; Source entry ID [%lli], Destination entry ID [%lli]; "
       "Period [%i hours]; Status [%lli]. "
       "From table ["%s"] of database ["%s"]",
       policy.getID(),
       policy.getName().c_str(),
       policy.getDescription().c_str(),
       policy.getRatio(),
       policy.getSourceID(),
       policy.getDestinationID(),
       policy.getPeriod(),
       static_cast<sqlite3_int64>(policy.getStatus()),
       this->m_policy_table.getName().c_str(),
       this->m_db_name.c_str());
  Entry source_entry = this->m_cycle_table.readEntry(policy.getSourceID());
  DBG3("Read source entry [ID: %lli] from table ["%s"].",
       source_entry.getID(), this->m_cycle_table.getName().c_str());
  MoneyValue_t value =
      Policy::calculateRatioOfBalance(
          source_entry.getBalance(),
          policy.getRatio());
  DBG3("Calculated ratio [%lli] of source entry balance [%lli]: %lli.",
       policy.getRatio(), source_entry.getBalance(), value);
  this->m_cycle_table.updateEntry(
      source_entry.getID(),
      -value,
      policy.getDescription());
  DBG3("Updated source entry [ID: %lli] for value %lli. "
       "Final balance [%lli].",
       source_entry.getID(), -value, source_entry.getBalance());
  Record record =
      this->update(
          policy.getDestinationID(),
          value,
          policy.getDescription());
  DBG3("Updated destination entry [ID: %lli] for value %lli.",
       policy.getDestinationID(), value);
  DBG3("Record corresponding to applied policy [ID: %lli]: "
       "Record ID [%lli]; Balance [%lli]; Description ["%s"]; "
       "Date ["%s"]; Time ["%s"]; Status [%lli].",
       policy.getID(),
       record.getID(),
       record.getBalance(),
       record.getDescription().c_str(),
       record.getDateTime().getDate().c_str(),
       record.getDateTime().getTime().c_str(),
       static_cast<sqlite3_int64>(record.getStatus()));
  INF("exit TableManager::applyPolicy().");
  return (record);
}

void TableManager::deletePolicy(const ID_t& i_policy_id) {
  INF("enter TableManager::deletePolicy().");
  this->m_policy_table.deletePolicy(i_policy_id);
  DBG3("Deleted policy [ID: %lli] from table ["%s"].",
       i_policy_id, this->m_policy_table.getName().c_str());
  INF("exit TableManager::deletePolicy().");
}

Entry TableManager::undoPolicy(const ID_t& i_policy_id) {
  INF("enter TableManager::undoPolicy().");
  std::shared_ptr<DateTime> ptr_datetime;
  Policy policy = this->m_policy_table.readPolicy(i_policy_id, ptr_datetime);
  // TODO: proper rollback source entry   // this->m_cycle_table.r
  Entry destination_entry = this->undo(policy.getDestinationID());
  /*DBG3("Undone policy [ID: %lli], removed record [ID: %lli] "
       "from table ["%s"].",
       i_policy_id, );*/
  INF("exit TableManager::undoPolicy().");
  return (destination_entry);
}

// ----------------------------------------------------------------------------
const std::string& TableManager::getCycleTableName() const {
  INF("enter TableManager::getCycleTableName().");
  DBG3("Cycle Table name is ["%s"].", this->m_cycle_table.getName().c_str());
  INF("exit TableManager::getCycleTableName().");
  return (this->m_cycle_table.getName());
}

const std::string& TableManager::getDailyTableName() const {
  INF("enter TableManager::getDailyTableName().");
  DBG3("Daily Table name is ["%s"].", this->m_daily_table.getName().c_str());
  INF("exit TableManager::getDailyTableName().");
  return (this->m_daily_table.getName());
}

const std::string& TableManager::getPolicyTableName() const {
  INF("enter TableManager::getPolicyTableName().");
  DBG3("Policy Table name is ["%s"].", this->m_policy_table.getName().c_str());
  INF("exit TableManager::getPolicyTableName().");
  return (this->m_policy_table.getName());
}


/* Private members */
// ----------------------------------------------------------------------------
void TableManager::__init__() {
  DBG4("enter TableManager::__init__().");
  iDatabase::__init__();
  std::string statement = "SELECT * FROM '" + this->m_table_name + "';";
  this->__prepare_statement__(statement);
  int result = sqlite3_step(this->m_db_statement);
  while (result == SQLITE_ROW) {
    ID_t entry_id = sqlite3_column_int64(this->m_db_statement, 0);
    this->m_entry_ids.insert(entry_id);
    DBG4("Inserted entry [ID: %lli] from table ["%s"].",
         entry_id, this->m_table_name.c_str());
    result = sqlite3_step(this->m_db_statement);
  }
  this->__finalize__(statement.c_str());
  DBG4("exit TableManager::__init__().");
}

void TableManager::__create_table__() {
  DBG4("enter TableManager::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += this->m_table_name;
  statement += "('EntryID' INTEGER PRIMARY KEY, 'RecordsTableName' TEXT);";
  this->__prepare_statement__(statement);
  sqlite3_step(this->m_db_statement);
  DBG4("Table ["%s"] has been successfully created.",
       this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG4("exit TableManager::__create_table__().");
}

void TableManager::__create_table_entry_records__(
    const std::string& i_table_name) {
  DBG4("enter TableManager::__create_table_entry_records__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += i_table_name;
  statement += "('RecordID' INTEGER PRIMARY KEY);";
  this->__prepare_statement__(statement);
  sqlite3_step(this->m_db_statement);
  DBG4("Table ["%s"] has been successfully created.",
       this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG4("exit TableManager::__create_table_entry_records__().");
}

}  /* namespace mw */
