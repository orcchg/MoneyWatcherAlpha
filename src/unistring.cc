/*
 * unistring.cc
 *
 *  Description: String wrapper for various locales implementation.
 *
 *  Created on: 08.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "unistring.h"


namespace mw {

/*WrappedString::WrappedString(const String_t& str)  // private
  : m_string(str) {
}*/

WrappedString::WrappedString(const std::string& ordinary_str)
  : m_string(ordinary_str.begin(), ordinary_str.end()) {
}

WrappedString::WrappedString() {
}

WrappedString::WrappedString(const void* raw_char_data)
  : m_string(static_cast<const Char_t*>(raw_char_data)) {
}

WrappedString::~WrappedString() {
}


size_t WrappedString::length() const {
  return (this->m_string.length());
}

int WrappedString::n_bytes() const {
  return (static_cast<int>(this->m_string.length()) * sizeof(Char_t) * 2);
}

const WrappedString::Char_t* WrappedString::c_str() const {
  return (this->m_string.c_str());
}


WrappedString& WrappedString::operator += (const WrappedString& rhs) {
  this->m_string += rhs.m_string;
  return (*this);
}


/* To string */
// ----------------------------------------------
WrappedString WrappedString::to_string (int val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (long val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (long long val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (unsigned val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (unsigned long val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (unsigned long long val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (float val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (double val) {
  return (WrappedString(__TO_STRING__(val)));
}

WrappedString WrappedString::to_string (long double val) {
  return (WrappedString(__TO_STRING__(val)));
}



/* WrappedString exception */
// ----------------------------------------------------------------------------
WrappedStringException::WrappedStringException(const char* i_message)
  : m_message(i_message) {
}

WrappedStringException::~WrappedStringException() throw() {
}

const char* WrappedStringException::what() const throw() {
  return (this->m_message);
}

}  /* namespace mw */
