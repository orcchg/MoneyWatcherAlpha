/*
 * policy_table.cc
 *
 *  Description: Table with records from applied policies, implementation.
 *
 *  Created on: 20.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "datetime.h"
#include "logger.h"
#include "policy_table.h"
#include "service.h"


namespace mw {

int PolicyTable::OPENED_POLICY_TABLES_COUNT = 0;

PolicyTable::PolicyTable(
    const std::string& i_db_name,
    const std::string& i_table_name)
  : iDatabase(i_db_name, i_table_name) {
  INF("enter PolicyTable constructor.");
  this->__init__();
  ++PolicyTable::OPENED_POLICY_TABLES_COUNT;
  INF("exit PolicyTable constructor.");
}

PolicyTable::PolicyTable(PolicyTable&& rval_obj)
  : iDatabase(std::move(static_cast<iDatabase&>(rval_obj))) {
}

PolicyTable::~PolicyTable() {
  INF("enter PolicyTable destructor.");
  this->__close_database__();
  --PolicyTable::OPENED_POLICY_TABLES_COUNT;
  INF("exit PolicyTable destructor.");
}

Policy PolicyTable::addPolicy(
    const WrappedString& i_name,
    const WrappedString& i_description,
    const PolicyRatio_t& i_ratio,
    const ID_t& i_source_entry_id,
    const ID_t& i_destination_entry_id,
    int i_hours_period,
    const PolicyStatus& i_status) {
  INF("enter PolicyTable::addPolicy().");
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_table_name;
  insert_statement += "' VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10);";
  this->__prepare_statement__(insert_statement);

  bool accumulate = true;
  ID_t policy_id = this->m_next_id++;
  accumulate = accumulate &&
      (sqlite3_bind_int64(this->m_db_statement, 1, policy_id) == SQLITE_OK);
  DBG1("ID [%lli] has been stored in table ["%s"], SQLite database ["%s"].",
       policy_id, this->m_table_name.c_str(), this->m_db_name.c_str());

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
  DBG1("Description ["%s"] has been stored in table ["%s"], "
       "SQLite database ["%s"].",
       i_description.c_str(),
       this->m_table_name.c_str(),
       this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          4,
          i_ratio) == SQLITE_OK);
  DBG1("Ratio [%lli] has been stored in table ["%s"], SQLite database ["%s"].",
       i_ratio, this->m_table_name.c_str(), this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          5,
          i_source_entry_id) == SQLITE_OK);
  DBG1("Source entry [ID: %lli] has been stored in table ["%s"], "
       "SQLite database ["%s"].",
       i_source_entry_id, this->m_table_name.c_str(), this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          6,
          i_destination_entry_id) == SQLITE_OK);
  DBG1("Destination entry [ID: %lli] has been stored in table ["%s"], "
       "SQLite database ["%s"].",
       i_destination_entry_id,
       this->m_table_name.c_str(),
       this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int(this->m_db_statement, 7, i_hours_period) == SQLITE_OK);
  DBG1("Hours [%lli] has been stored in table ["%s"], SQLite database ["%s"].",
       i_hours_period, this->m_table_name.c_str(), this->m_db_name.c_str());

  DateTime current_datetime;
  std::string date = current_datetime.getDate();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          8,
          date.c_str(),
          date.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG1("Date ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
       date.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  std::string time = current_datetime.getTime();
  accumulate = accumulate &&
      (sqlite3_bind_text(
          this->m_db_statement,
          9,
          time.c_str(),
          time.length(),
          SQLITE_TRANSIENT) == SQLITE_OK);
  DBG1("Time ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
       time.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  accumulate = accumulate &&
      (sqlite3_bind_int64(
          this->m_db_statement,
          10,
          static_cast<sqlite3_int64>(i_status)) == SQLITE_OK);
  DBG1("Status [%lli] has been stored in table ["%s"], "
       "SQLite database ["%s"].",
       static_cast<sqlite3_int64>(i_status),
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
    DBG1("All insertions have succeeded.");
  }

#if ENABLED_DB_CACHING
  // TODO: caching the policy
#endif

  this->__finalize__(insert_statement.c_str());
  this->__increment_rows__();
  Policy policy(
      policy_id,
      i_name,
      i_description,
      i_ratio,
      i_source_entry_id,
      i_destination_entry_id,
      i_hours_period,
      i_status);
  DBG1("Constructed output entry.");

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit PolicyTable::addPolicy().");
  return (policy);
}

Policy PolicyTable::readPolicy(
    const ID_t& i_policy_id,
    std::shared_ptr<DateTime> o_datetime) {
  INF("enter PolicyTable::readPolicy().");

#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_policy_id);
#endif

  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE ID == '";
  select_statement += std::to_string(i_policy_id);
  select_statement += "';";
  this->__prepare_statement__(select_statement);
  sqlite3_step(this->m_db_statement);
  ID_t id = sqlite3_column_int64(this->m_db_statement, 0);
  DBG1("Read id [%lli] from  table ["%s"] of database ["%s"], "
       "input id was [%lli].",
       id, this->m_table_name.c_str(), this->m_db_name.c_str(), i_policy_id);
  TABLE_ASSERT("Input entry id does not equal to primary key value "
               "from database!" &&
               id == i_policy_id);

  const void* raw_name = reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 1));
  WrappedString name(raw_name);
  const void* raw_description = reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 2));
  WrappedString description(raw_description);
  PolicyRatio_t ratio = sqlite3_column_int64(this->m_db_statement, 3);
  ID_t source_entry_id = sqlite3_column_int64(this->m_db_statement, 4);
  ID_t destination_entry_id = sqlite3_column_int64(this->m_db_statement, 5);
  int hours_period = sqlite3_column_int(this->m_db_statement, 6);
  std::string date(reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 7)));
  std::string time(reinterpret_cast<const char*>(
      sqlite3_column_text(this->m_db_statement, 8)));
  o_datetime = std::make_shared<DateTime>(date, time);
  sqlite3_int64 raw_status = sqlite3_column_int64(this->m_db_statement, 9);
  PolicyStatus status(raw_status);
  DBG1("Loaded column data: Name ["%s"]; Description ["%s"]; Ratio [%lli]; "
       "Source entry [ID: %lli]; Destination entry [ID: %lli]; "
       "Period [%i]; Date ["%s"]; Time ["%s"]; Status [%lli].",
       name.c_str(),
       description.c_str(),
       ratio,
       source_entry_id,
       destination_entry_id,
       hours_period,
       o_datetime->getDate().c_str(),
       o_datetime->getTime().c_str(),
       raw_status);
  Policy policy(
      id,
      name,
      description,
      ratio,
      source_entry_id,
      destination_entry_id,
      hours_period,
      status);
  DBG1("Proper policy instance has been constructed.");

#if ENABLED_DB_CACHING
  // TODO: caching the policy
#endif

  this->__finalize__(select_statement.c_str());
  INF("exit PolicyTable::readPolicy().");
  return (policy);
}

void PolicyTable::deletePolicy(const ID_t& i_policy_id) {
  INF("enter PolicyTable::deletePolicy().");
#if ENABLED_ADVANCED_DEBUG
  this->__where_check__(i_policy_id);
#endif

  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID == '";
  delete_statement += std::to_string(i_policy_id);
  delete_statement += "';";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  this->__decrement_rows__();
  if (i_policy_id + 1 == this->m_next_id) {
    ID_t last_row_id = this->__read_last_id__(this->m_table_name);
    this->m_next_id = last_row_id + 1;
    DBG1("Deleted policy with largest ID. Next ID value is set to [%lli].",
         this->m_next_id);
  }
  if (this->__empty__()) {
    DBG1("Table ["%s"] has become empty. Next ID value is set to zero.",
         this->m_table_name.c_str());
    this->m_next_id = 0;
  }
  DBG1("Deleted policy [ID: %lli] in table ["%s"].",
       i_policy_id, this->m_table_name.c_str());

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit PolicyTable::deletePolicy().");
}

void PolicyTable::deletePolicies(std::vector<ID_t>& i_policy_ids) {
  INF("enter PolicyTable::deletePolicies().");
  std::sort(i_policy_ids.begin(), i_policy_ids.end());

#if ENABLED_ADVANCED_DEBUG
  for (ID_t& policy_id : i_policy_ids) {
    this->__where_check__(policy_id);
  }
#endif

  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID IN(";
  delete_statement += vectorToString(i_policy_ids, ", ");
  delete_statement += ");";
  this->__prepare_statement__(delete_statement);
  sqlite3_step(this->m_db_statement);
  this->__finalize__(delete_statement.c_str());
  this->__decrease_rows__(static_cast<int>(i_policy_ids.size()));
  DBG1("Updating next ID if necessary, initial value is [%lli].",
       this->m_next_id);
  for (std::vector<ID_t>::const_reverse_iterator it = i_policy_ids.rbegin();
       it != i_policy_ids.rend();
       ++it) {
    if (*it + 1 == this->m_next_id) {
      --this->m_next_id;
      DBG1("Deleted policy with largest ID. "
           "Next ID value has been decremented.");
    } else {
      break;
    }
  }
  ID_t last_row_id = this->__read_last_id__(this->m_table_name);
  this->m_next_id = last_row_id + 1;
  DBG1("Deleted policy with largest ID. Next ID value is set to [%lli].",
       this->m_next_id);
  DBG1("Finished updating next ID: [%lli].", this->m_next_id);
  if (this->__empty__()) {
    DBG1("Table ["%s"] has become empty. Next id value is set to zero.",
         this->m_table_name.c_str());
    this->m_next_id = 0;
  }
  DBG1("Deleted %lli policies from table ["%s"].",
       static_cast<long long int>(i_policy_ids.size()),
       this->m_table_name.c_str());

#if ENABLED_ADVANCED_DEBUG
  this->__count_check__();
#endif

  INF("exit PolicyTable::deletePolicies().");
}

const std::string& PolicyTable::getName() const {
  INF("enter PolicyTable::getName().");
  DBG1("Return the name ["%s"] of PolicyTable at %p.",
       this->m_table_name.c_str(), this);
  INF("exit PolicyTable::getName().");
  return (this->m_table_name);
}


/* Private members */
// ----------------------------------------------------------------------------
void PolicyTable::__init__() {
  DBG2("enter PolicyTable::__init__().");
  iDatabase::__init__();
  ID_t last_row_id = this->__read_last_id__(this->m_table_name);
  this->m_next_id = last_row_id == 0 ? 0 : last_row_id + 1;
  TRC("Initialization has completed: total rows [%i], last row id [%lli], "
      "next_id [%lli].",
      this->m_rows, last_row_id, this->m_next_id);
  DBG2("exit PolicyTable::__init__().");
}

void PolicyTable::__create_table__() {
  DBG2("enter PolicyTable::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += this->m_table_name;
  statement += "('ID' INTEGER PRIMARY KEY, "
      "'Name' TEXT, "
      "'Description' TEXT, "
      "'Ratio' INTEGER, "
      "'SourceEntryID' INTEGER, "
      "'DestinationEntryID' INTEGER, "
      "'Period' INTEGER, "
      "'Date' TEXT, "
      "'Time' TEXT, "
      "'Status' INTEGER);";
  this->__prepare_statement__(statement);
  sqlite3_step(this->m_db_statement);
  DBG2("Table ["%s"] has been successfully created.",
       this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG2("exit PolicyTable::__create_table__().");
}

}  /* namespace mw */
