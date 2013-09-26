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

#include "serializable.h"
#include "types.h"


namespace mw {

/// @class Status
/// @brief Represents various statuses of data.
class Status : public iSerializable {
public:
  Status(const Status_t& status);
  virtual ~Status();

  const Status_t& getStatus() const;
  void setStatus(const Status_t& status);

  void serialize(std::string* output) const;

private:
  Status_t m_status;
};

}  /* namespace mw */

#endif /* STATUS_H_ */
