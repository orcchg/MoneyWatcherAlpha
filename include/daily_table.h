/*
 * daily_table.h
 *
 *  Description: Daily Table class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef DAILY_TABLE_H_
#define DAILY_TABLE_H_

#include <unordered_map>
#include "hash.h"
#include "record.h"


namespace mw {

/// @class DailyTable
/// @brief Represents a table of daily changes.
class DailyTable {
 public:

 private:
  std::unordered_map<ID_t, Record, Hasher<ID_t> > m_records;
};

}  /* namespace mw */

#endif /* DAILY_TABLE_H_ */
