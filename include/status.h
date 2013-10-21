/*
 * status.h
 *
 *  Description: Status class declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef STATUS_H_
#define STATUS_H_

#include "types.h"


namespace mw {

enum StatusValue { SV_UNKNOWN = -1,
	                 SV_EXPENSE = 0,
	                 SV_INCOME = 1,
	                 SV_COUNT
};

/// @class Status
/// @brief Represents various statuses of data.
class Status {
public:
  Status(const sqlite3_int64& status);
  virtual ~Status();

  const StatusValue& getStatus() const;
  void setStatus(const StatusValue& status);

  operator sqlite3_int64() const;
  bool operator == (const Status& rhs) const;
  bool operator != (const Status& rhs) const;

private:
  StatusValue m_status;
};

}  /* namespace mw */

#endif /* STATUS_H_ */
