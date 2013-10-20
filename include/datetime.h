/*
 * datetime.h
 *
 *  Description: Date & Time methods declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef DATETIME_H_
#define DATETIME_H_

#include <exception>
#include <string>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#define DATE_TIME_EXCEPTION_ERROR_CODE -3


namespace mw {

/// @class DateTime
/// @brief Class always represents current Date and
/// Time when it's instance is constructed.
class DateTime {
public:
  /// @brief Record current Date and Time into class' new instance.
  /// @note Date and Time cannot be changed after construction.
  DateTime();

  /// @brief Creates new class' instance with date and time
  /// being set in advance.
  /// @note Date and Time cannot be changed after construction.
  DateTime(const std::string& date, const std::string& time);

  DateTime(const DateTime& obj);
  DateTime(DateTime&& rval_obj);
  virtual ~DateTime();

  DateTime& operator = (const DateTime& rhs);
  DateTime& operator = (DateTime&& rval_rhs);

  const std::string& getDate() const;
  const std::string& getTime() const;
  const std::string& getWeekDay() const;

  /// -------------------------------------------------------------------------
  /// @defgroup COMP Compare two date-time instances.
  /// @{
  bool operator < (const DateTime& rhs) const;
  bool operator <= (const DateTime& rhs) const;
  bool operator > (const DateTime& rhs) const;
  bool operator >= (const DateTime& rhs) const;
  bool operator == (const DateTime& rhs) const;
  bool operator != (const DateTime& rhs) const;
  /// @}
  /// -------------------------------------------------------------------------

  /// -------------------------------------------------------------------------
  /// @defgroup ARITHMETICS Date-time arithmetics.
  /// @{
  /// @brief Adds given number of hours to the current date-time, providing
  /// new DateTime instances with value of current date-time plus time period.
  /// @param hours - Number of hours.
  /// @return New date-time.
  DateTime operator + (int hours) const;
  /// @}
  /// -------------------------------------------------------------------------

private:
  boost::posix_time::ptime m_datetime;
  std::string m_yearMonthDay;
  std::string m_hoursMinutesSeconds;
  std::string m_weekday;

  void __parse__(
      std::string& ymd,
      std::string& hms,
      std::string& weekday) const;
};


// ----------------------------------------------------------------------------
/// @class DateTimeException
/// @brief Represents a common exception raised by DateTime class methods.
class DateTimeException : public std::exception {
public:
  DateTimeException(const char* message, int error_code);
  virtual ~DateTimeException() throw();

  const char* what() const throw();
  int error() const throw();

private:
  const char* m_message;
  int m_error_code;
};

}  /* namespace mw */

#endif /* DATETIME_H_ */
