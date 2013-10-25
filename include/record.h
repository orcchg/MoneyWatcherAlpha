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
#include "unistring.h"


namespace mw {

/// @class Record
/// @brief Represents a single record in Daily Table.
class Record {
public:
  Record(
      const ID_t& id,
      const MoneyValue_t& balance,
      const WrappedString& description,
      const RecordStatus& status,
      const DateTime& datetime = DateTime());
  virtual ~Record();

  /// -------------------------------------------------------------------------
  /// @defgroup GET Getters for class members.
  /// @{
  /// @brief Gets an ID of the current record.
  ID_t getID() const;
  /// @brief Gets an actual money balance of the current record.
  const MoneyValue_t& getBalance() const;
  /// @brief Gets the descriptive comment supplied with the
  /// current record.
  const WrappedString& getDescription() const;
  /// @brief Gets the date and the clock time, when the current record
  /// has been created.
  const DateTime& getDateTime() const;
  /// @brief Gets the status of the current record.
  const RecordStatus& getStatus() const;
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup SET Setters for class members.
  /// @{
  /// @brief Sets status of current record to APPLIED_POLICY.
  void setAppliedPolicyStatus();
  /// @}
  /// -------------------------------------------------------------------------

private:
  ID_t m_id;
  MoneyValue_t m_balance;
  WrappedString m_description;
  DateTime m_datetime;
  RecordStatus m_status;
};

} /* namespace mw */

#endif /* RECORD_H_ */
