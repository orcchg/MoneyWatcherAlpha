/*
 * entry.h
 *
 *  Description: Entry class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef ENTRY_H_
#define ENTRY_H_

#include <string>
#include "datetime.h"
#include "status.h"
#include "types.h"


namespace mw {

/// @struct Entry
/// @brief Represents an element of Cycle Table.
struct Entry {
  ID_t id;
  std::string name;
  MoneyValue_t money;
  DateTime datetime;
  MoneyValue_t last_transaction;
  Status status;
  void* data;
};

}  /* namespace mw */

#endif /* ENTRY_H_ */
