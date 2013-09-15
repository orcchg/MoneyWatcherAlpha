/*
 * cycle_table.h
 *
 *  Description: Cycle Table class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef CYCLE_TABLE_H_
#define CYCLE_TABLE_H_

#include <string>
#include <unordered_map>
#include "entry.h"
#include "hash.h"


namespace mw {

/// @class CycleTable
/// @brief Represents a table of entries, updating once per certain cycle.
class CycleTable {
public:
  void addEntry(const std::string& name);
  Entry& getEntry(ID_t entry_id) const;

private:
  std::unordered_map<ID_t, Entry, Hasher<ID_t> > m_entries;
};

}  /* namespace mw */

#endif /* CYCLE_TABLE_H_ */
