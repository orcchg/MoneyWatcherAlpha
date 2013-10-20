/*
 * datetime.cc
 *
 *  Description: Date & Time implementation.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <algorithm>
#include <string>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "datetime.h"


namespace mw {

DateTime::DateTime()
  : m_datetime(boost::posix_time::second_clock::local_time())
  , m_yearMonthDay("")
  , m_hoursMinutesSeconds("")
  , m_weekday("") {
  this->__parse__(
      this->m_yearMonthDay,
      this->m_hoursMinutesSeconds,
      this->m_weekday);
}

DateTime::DateTime(const std::string& date, const std::string& time)
  : m_datetime(boost::posix_time::time_from_string(date + " " + time))
  , m_yearMonthDay(date)
  , m_hoursMinutesSeconds(time) {
  std::string ymd, hms;
  this->__parse__(ymd, hms, this->m_weekday);
  if (ymd != m_yearMonthDay) {
    throw DateTimeException(
        "Parse error of date!",
        DATE_TIME_EXCEPTION_ERROR_CODE);
  }
  if (hms != m_hoursMinutesSeconds) {
    throw DateTimeException(
        "Parse error of time!",
        DATE_TIME_EXCEPTION_ERROR_CODE);
  }
}

DateTime::DateTime(const DateTime& obj)
  : m_datetime(obj.m_datetime)
  , m_yearMonthDay(obj.m_yearMonthDay)
  , m_hoursMinutesSeconds(obj.m_hoursMinutesSeconds)
  , m_weekday(obj.m_weekday) {
}

DateTime::DateTime(DateTime&& rval_obj)
  : m_datetime(rval_obj.m_datetime)
  , m_yearMonthDay(rval_obj.m_yearMonthDay)
  , m_hoursMinutesSeconds(rval_obj.m_hoursMinutesSeconds)
  , m_weekday(rval_obj.m_weekday) {
}

DateTime::~DateTime() {
}

DateTime& DateTime::operator = (const DateTime& rhs) {
  DateTime temp(rhs);
  std::swap(this->m_datetime, temp.m_datetime);
  std::swap(this->m_yearMonthDay, temp.m_yearMonthDay);
  std::swap(this->m_hoursMinutesSeconds, temp.m_hoursMinutesSeconds);
  std::swap(this->m_weekday, temp.m_weekday);
  return (*this);
}

DateTime& DateTime::operator = (DateTime&& rval_rhs) {
  DateTime moved_rhs(rval_rhs);
  std::swap(this->m_datetime, moved_rhs.m_datetime);
  std::swap(this->m_yearMonthDay, moved_rhs.m_yearMonthDay);
  std::swap(this->m_hoursMinutesSeconds, moved_rhs.m_hoursMinutesSeconds);
  std::swap(this->m_weekday, moved_rhs.m_weekday);
  return (*this);
}

const std::string& DateTime::getDate() const {
  return (this->m_yearMonthDay);
}

const std::string& DateTime::getTime() const {
  return (this->m_hoursMinutesSeconds);
}

const std::string& DateTime::getWeekDay() const {
  return (this->m_weekday);
}

bool DateTime::operator < (const DateTime& rhs) const {
  return (this->m_datetime < rhs.m_datetime);
}

bool DateTime::operator <= (const DateTime& rhs) const {
  return (this->m_datetime <= rhs.m_datetime);
}

bool DateTime::operator > (const DateTime& rhs) const {
  return (this->m_datetime > rhs.m_datetime);
}

bool DateTime::operator >= (const DateTime& rhs) const {
  return (this->m_datetime >= rhs.m_datetime);
}

bool DateTime::operator == (const DateTime& rhs) const {
  return (this->m_datetime == rhs.m_datetime);
}

bool DateTime::operator != (const DateTime& rhs) const {
  return (this->m_datetime != rhs.m_datetime);
}

DateTime DateTime::operator + (int i_hours) const {
  DateTime result(*this);
  result.m_datetime += boost::posix_time::hours(i_hours);
  this->__parse__(
      result.m_yearMonthDay,
      result.m_hoursMinutesSeconds,
      result.m_weekday);
  return (result);
}

void DateTime::__parse__(
    std::string& ymd,
    std::string& hms,
    std::string& weekday) const {
  std::ostringstream oss;
  oss << m_datetime.date().year()
      << "-" << m_datetime.date().month()
      << "-" << m_datetime.date().day();
  if (!oss.good()) {
    throw DateTimeException(
        "Error during date processing!",
        DATE_TIME_EXCEPTION_ERROR_CODE);
  }
  ymd = oss.str();
  oss.str("");
  oss.flush();
  oss.clear();

  oss << m_datetime.time_of_day().hours() << ":"
      << m_datetime.time_of_day().minutes() << ":"
      << m_datetime.time_of_day().seconds();
  if (!oss.good()) {
    throw DateTimeException(
        "Error during time of day processing!",
        DATE_TIME_EXCEPTION_ERROR_CODE);
  }
  hms = oss.str();
  oss.str("");
  oss.flush();
  oss.clear();

  oss << m_datetime.date().day_of_week();
  if (!oss.good()) {
    throw DateTimeException(
        "Error during week day processing!",
        DATE_TIME_EXCEPTION_ERROR_CODE);
  }
  weekday = oss.str();
  oss.str("");
  oss.flush();
  oss.clear();
}


/* DateTime exception */
// ----------------------------------------------------------------------------
DateTimeException::DateTimeException(const char* i_message, int i_error_code)
  : m_message(i_message)
  , m_error_code(i_error_code) {
}

DateTimeException::~DateTimeException() throw() {
}

const char* DateTimeException::what() const throw() {
  return (this->m_message);
}

int DateTimeException::error() const throw() {
  return (this->m_error_code);
}

}  /* namespace mw */
