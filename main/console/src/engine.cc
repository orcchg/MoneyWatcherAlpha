/*
 * engine.cc
 *
 *  Description: Console application engine implementation.
 *
 *  Created on: 19.01.2014
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <unordered_map>
#include <utility>
#include <cstdio>
#include "engine.h"
#include "logger.h"


namespace mw {

/* Public API */
// ----------------------------------------------------------------------------
void ConsoleEngine::run() {
  this->__banner__();
  this->__show_menu__();

  char key = '.';
  bool loop_flag = true;
  while (loop_flag) {
    key = getchar();
    switch (key) {
      case 'c':
        INF("Pressed 'C' in main loop: Cycle table.");
        loop_flag = !this->__list_cycle_table__();
        break;
      case 'd':
        INF("Pressed 'D' in main loop: Daily table.");
        this->__list_daily_table__();
        break;
      case 'q':
        INF("Pressed 'Q' in main loop: Exit.");
        return;
      default:
        INF("Pressed some key: Nothing to do.");
        break;
    }
  }

  this->__run_cycle_table_operations__();
}


/* Private members */
// ----------------------------------------------------------------------------
void ConsoleEngine::__banner__() {
  printf("\n\n");
  printf("****************************************************************\n");
  printf("*                                                              *\n");
  printf("*            Money Watcher.  Version 1.0                       *\n");
  printf("*                                                              *\n");
  printf("****************************************************************\n");
  printf("\n\n");
}

void ConsoleEngine::__show_menu__() {
  printf("Cycle Table  ................................................  C\n");
  printf("Daily Table  ................................................  D\n");
  printf("Exit  .......................................................  Q\n");
  printf("\n\n");
}

bool ConsoleEngine::__list_cycle_table__() {
  const std::unordered_map<ID_t, Entry> entries =
      this->m_table_manager.listCycleTable();
  printf("Cycle Table listing, Count: %lu.\n\n", entries.size());
  for (const std::pair<ID_t, Entry>& entry_pair : entries) {
    printf("E[%lli]:  \"%s\" : \"%s\" : %lli : %lli : (\"%s:%s\") : %lli\n",
           entry_pair.first,
           entry_pair.second.getName().c_str(),
           entry_pair.second.getDescription().c_str(),
           entry_pair.second.getBalance(),
           entry_pair.second.getLastTransaction(),
           entry_pair.second.getDateTime().getDate().c_str(),
           entry_pair.second.getDateTime().getTime().c_str(),
           static_cast<sqlite3_int64>(entry_pair.second.getStatus()));
  }
  return (true);
}

void ConsoleEngine::__list_daily_table__() {
  const std::unordered_map<ID_t, Record> records =
      this->m_table_manager.listDailyTable();
  printf("Daily Table listing, Count: %lu\n\n", records.size());
  for (const std::pair<ID_t, Record>& record_pair : records) {
    printf("R[%lli]:  \"%s\" : %lli : (\"%s:%s\") : %lli\n",
           record_pair.first,
           record_pair.second.getDescription().c_str(),
           record_pair.second.getBalance(),
           record_pair.second.getDateTime().getDate().c_str(),
           record_pair.second.getDateTime().getTime().c_str(),
           static_cast<sqlite3_int64>(record_pair.second.getStatus()));
  }
}

void ConsoleEngine::__run_cycle_table_operations__() {
  //
}

}  /* namespace mw */
