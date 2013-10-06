/*
 * service.cc
 *
 *  Description: Service functions declaration.
 *
 *  Created on: 07.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "service.h"


std::wstring widenString(const std::string& src) {
  std::wstring dest(src.begin(), src.end());
  return (dest);
}
