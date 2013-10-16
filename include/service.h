/*
 * service.h
 *
 *  Description: Service functions declaration.
 *
 *  Created on: 07.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <string>
#include <vector>
#include "logger.h"


std::wstring widenString(const std::string& src);

template <typename T>
std::string vectorToString(const std::vector<T>& array, const std::string& delim = ", ") {
  std::string result = "";
  if (array.empty()) {
    return (result);
  }
  for (const T& item : array) {
    result += std::to_string(item);
    result += delim;
  }
  size_t length = delim.length();
  for (size_t index = 0; index < length; ++index) {
    result.pop_back();
  }
  MSG("String: ["%s"].", result.c_str());
  return result;
}

#endif /* SERVICE_H_ */
