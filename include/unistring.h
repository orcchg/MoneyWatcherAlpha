/*
 * unistring.h
 *
 *  Description: String wrapper for various locales declaration.
 *
 *  Created on: 08.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef UNISTRING_H_
#define UNISTRING_H_

#include <exception>
#include <string>

#define __TO_STRING__(val) (std::to_string(val))


namespace mw {

class WrappedString {
  typedef std::string::value_type Char_t;
  typedef std::string String_t;
  //WrappedString(const String_t& str);

public:
  WrappedString();
  WrappedString(const std::string& ordinary_str);
  WrappedString(const void* raw_char_data);
  virtual ~WrappedString();

  size_t length() const;
  int n_bytes() const;
  const Char_t* c_str() const;

  WrappedString& operator += (const WrappedString& rhs);

  static WrappedString to_string (int val);
  static WrappedString to_string (long val);
  static WrappedString to_string (long long val);
  static WrappedString to_string (unsigned val);
  static WrappedString to_string (unsigned long val);
  static WrappedString to_string (unsigned long long val);
  static WrappedString to_string (float val);
  static WrappedString to_string (double val);
  static WrappedString to_string (long double val);

private:
  String_t m_string;
};

/// @class WrappedStringException
/// @brief Represents a common exception raised by WrappedString class methods.
class WrappedStringException : public std::exception {
public:
  WrappedStringException(const char* message);
  virtual ~WrappedStringException() throw();

  const char* what() const throw();

private:
  const char* m_message;
};

}  /* namespace mw */

#endif /* UNISTRING_H_ */
