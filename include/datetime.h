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

#include <string>
#include <boost/date_time/posix_time/posix_time_types.hpp>


namespace mw {

/// @class DateTime
/// @brief Class always represents current Date and Time when it's instance is constructed.
class DateTime {
public:
  /// @brief Record current Date and Time into class' new instance.
  /// @note Date and Time cannot be changed after construction.
  DateTime();

  std::string getDate() const;
  std::string getTime() const;

  /// @defgroup COMP Compare two date-time instances.
  /// @{
  bool operator < (const DateTime& rhs) const;
  bool operator == (const DateTime& rhs) const;
  bool operator != (const DateTime& rhs) const;
  /// @}

private:
  boost::posix_time::ptime _datetime;
  std::string _dayMonthYearWeekday;
  std::string _hoursMinutesSeconds;
};

}  /* namespace mw */

#endif /* DATETIME_H_ */
