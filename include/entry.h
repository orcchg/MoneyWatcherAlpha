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
#include "record.h"
#include "status.h"
#include "types.h"
#include "unistring.h"


namespace mw {

/// @class Entry
/// @brief Represents an element of Cycle Table.
class Entry {
public:
  Entry(
      const ID_t& id,
      const WrappedString& name,
      const WrappedString& description,
      const MoneyValue_t& current_balance,
      const MoneyValue_t& last_transaction,
      const Status& status,
      const DateTime& datetime = DateTime());
  virtual ~Entry();

  /// -------------------------------------------------------------------------
  /// @defgroup GET Getters for class members.
  /// @{
  /// @brief Gets an ID of the current entry.
  ID_t getID() const;
  /// @brief Gets a name of the current entry.
  const WrappedString& getName() const;
  /// @brief Gets the descriptive comment supplied with the
  /// last transaction.
  const WrappedString& getDescription() const;
  /// @brief Gets an actual money balance of the current entry.
  const MoneyValue_t& getBalance() const;
  /// @brief Gets a money value of the last transaction
  /// being performed on the current entry.
  const MoneyValue_t& getLastTransaction() const;
  /// @brief Gets the date and the clock time, when the
  /// last transaction of the current entry has been performed.
  const DateTime& getDateTime() const;
  /// @brief Gets the status of the last transaction.
  const Status& getStatus() const;
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup SET Setters for class members.
  /// @{
  /// @brief Updates an actual money balance of the current entry.
  /// @param value - Amount to which the actual money balance should
  /// be updated - it could be both positive (income) or negative (expense)
  /// value.
  /// @param description - Description of provided transaction.
  /// @return Status of last transaction.
  /// @details This action will generate an event for the Update Manager.
  Status updateBalance(const MoneyValue_t& value, const WrappedString& description);
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup SERVICE Various service functions.
  /// @{
  /// @brief Cancels the last action performed on the current entry
  /// and rolls the state of the entry back for the previous one.
  /// @note This action cannot be undone.
  void undo();

  /// @brief Clears the data of the current entry.
  /// @details This action will generate an event for the Update Manager.
  /// @note After this action the current entry will become empty,
  /// so the updateBalance() function should be invoked after.
  void clear();
  /// @}
  /// -------------------------------------------------------------------------

private:
  ID_t m_id;
  WrappedString m_name;
  WrappedString m_description;
  MoneyValue_t m_current_balance;
  MoneyValue_t m_last_transaction;
  DateTime m_datetime;
  Status m_status;

  /// @brief Makes record from current entry.
  /// @return Newly created record.
  Record __make_record__() const;
};

}  /* namespace mw */

#endif /* ENTRY_H_ */
