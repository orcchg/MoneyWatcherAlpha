/*
 * Record.h
 *
 *  Description: Record class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <string>
#include "datetime.h"
#include "status.h"
#include "types.h"


namespace mw {

/// @struct Record
/// @brief Represents a single record in Daily Table.
struct Record {
  ID_t id;
  std::string name;
  MoneyValue_t money;
  DateTime datetime;
  std::string description;
  Status status;
  void* data;
};

} /* namespace mw */

#endif /* RECORD_H_ */
