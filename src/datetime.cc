/*
 * datetime.cc
 *
 *  Description: Date & Time implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "datetime.h"


namespace mw {

DateTime::DateTime()
  : _datetime(boost::posix_time::second_clock::local_time()) {
  std::ostringstream oss;
  oss << _datetime.date().day() << " " << _datetime.date().month()
      << " " << _datetime.date().year() << " " << _datetime.date().day_of_week();
  if (!oss.good()) {
  throw std::runtime_error("Error during date processing!");
  }
  _dayMonthYearWeekday = oss.str();
  oss.str("");
  oss.flush();
  oss.clear();
  oss << _datetime.time_of_day().hours() << ":"
      << _datetime.time_of_day().minutes() << ":"
      << _datetime.time_of_day().seconds();
  if (!oss.good()) {
    throw std::runtime_error("Error during time of day processing!");
  }
  _hoursMinutesSeconds = oss.str();
  oss.str("");
  oss.flush();
  oss.clear();
}

std::string DateTime::getDate() const {
  return (_dayMonthYearWeekday);
}

std::string DateTime::getTime() const {
  return (_hoursMinutesSeconds);
}

bool DateTime::operator < (const DateTime& rhs) const {
  return (_datetime < rhs._datetime);
}

bool DateTime::operator == (const DateTime& rhs) const {
  return (_datetime == rhs._datetime);
}

bool DateTime::operator != (const DateTime& rhs) const {
  return (!(*this == rhs));
}

}  /* namespace mw */
