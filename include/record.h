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
#include "common.h"
#include "datetime.h"
#include "status.h"
#include "types.h"


namespace mw {

/// @class Record
/// @brief Represents a single record in Daily Table.
class Record {
public:
  Record(
      const MoneyValue_t& balance,
      const std::wstring& description,
      const Status& status,
      const DateTime& datetime = DateTime());
  virtual ~Record();

  // --------------------------------------------
  /// @defgroup GET Getters for class members.
  /// @{

  /// @brief Gets an ID of the current record.
  ID_t getID() const;
  /// @brief Gets an actual money balance of the current record.
  const MoneyValue_t& getBalance() const;
  /// @brief Gets the descriptive comment supplied with the
  /// current record.
  const std::wstring& getDescription() const;
  /// @brief Gets the date and the clock time, when the current record
  /// has been created.
  const DateTime& getDateTime() const;
  /// @brief Gets the status of the current record.
  const Status& getStatus() const;
  /// @}

private:
  ID_t m_id;
  MoneyValue_t m_balance;
  std::wstring m_description;
  DateTime m_datetime;
  Status m_status;
};

} /* namespace mw */

#endif /* RECORD_H_ */
