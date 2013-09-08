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

/// @class Record
/// @brief Represents a single record in Daily Table.
class Record {
 public:
  // --------------------------------------------
  /// @defgroup GET Getters for class members.
  /// @{

  /// @brief Gets an ID of the current record.
  ID_t getID() const;
  /// @brief Gets a name of the current record.
  const std::string& getName() const;
  /// @brief Gets an actual money balance of the current record.
  const MoneyValue_t& getBalance() const;
  /// @brief Gets the descriptive comment supplied with the
  /// current record.
  const std::string& getDescription() const;
  /// @brief Gets the date and the clock time, when the current record
  /// has been created.
  const DateTime& getDateTime() const;
  /// @brief Gets the status of the current record.
  const Status& getStatus() const;
  /// @brief Gets other data.
  void* getData() const;
  /// @}

 private:
  ID_t m_id;
  std::string m_name;
  MoneyValue_t m_balance;
  std::string m_description;
  DateTime m_datetime;
  Status m_status;
  void* m_data;
};

} /* namespace mw */

#endif /* RECORD_H_ */
