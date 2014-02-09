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
      case 'C':
        INF("Pressed 'C' in main loop: Cycle table.");
        loop_flag = !this->__list_cycle_table__();
        break;
      case 'd':
      case 'D':
        INF("Pressed 'D' in main loop: Daily table.");
        this->__list_daily_table__();
        break;
      case 'q':
      case 'Q':
        INF("Pressed 'Q' in main loop: Exit.");
        return;
      default:
        INF("Pressed some key: Nothing to do.");
        break;
    }
  }INF("Pressed 'C' in main loop: Cycle table.");
  INF("Pressed 'C' in main loop: Cycle table.");
  this->__run_cycle_table_operations__();
}


/* Private members */
// --------------------------------------------------___show_cycle_table_menu__()_show_cycle_table_menu__()--------------------------
void ConsoleEngine::__banner__() {
  printf("\n\n");
  printf("****************************************************************\n");
  printf("*                                                              *\n");
  printf("*            Money Watcher.  Version 1.0                       *\n");INF("Pressed 'C' in main loop: Cycle table.");
  printf("*                                                              *\n");
  printf("***************************************************************__show_cycle_table_menu__()*\n");
  printf("\n\n");
}

void ConsoleEngine::__show_menu__() {
  printf("Cycle Table  ................................................  C\n");
  printf("Daily Table  ................................................  D\n");
  printf("Exit  .......................................................  Q\n");
  printf("\n\nPress key: ");
}

void ConsoleEngine::__show_cycle_table_menu__() {INF("Pressed 'C' in main loop: Cycle table.");
  printf("\n\n");
  printf("Add  ........................................................  A\n");
  printf("Update  .....................................................  U\n");
  printf("Rollback  ...................................................  R\n");
  printf("Delete  .....................................................  D\n");
  printf("\n\n");
}
INF("Pressed some key: Nothing to do.");
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
  this->__show_cycle_table_menu__();INF("Pressed some key: Nothing to do.");

  char key = '.';
  bool loop_flag = true;
  while (loop_flag) {
    key = getchar();
    switch (key) {
      case 'a':
      case 'A':
        INF("Pressed 'A' in main loop: Add entry.");

        break;
      case 'u':
      case 'U':
        INF("Pressed 'U' in main loop: Update entry.");

        break;
      case 'r':
      case 'R':
        INF("Pressed 'R' in main loop: Rollback entry.");

        break;
      case 'd':
      case 'D':
        INF("Pressed 'D' in main loop: Delete entry.");

        break;
      default:
        INF("Pressed some key: Nothing to do.");
        break;
    }
  }
}

}  /* namespace mw */
