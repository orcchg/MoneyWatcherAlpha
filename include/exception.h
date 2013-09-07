/*
 * exception.h
 *
 *  Description:
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <string>


namespace mw {
namespace exception {

class Exception : public std::exception {
public:
  Exception(const std::string& message)
    : m_message(message) {
  }

  const char* what() const throw() {
    return (m_message.c_str());
  }

private:
  std::string m_message;
};

}  /* namespace exception */
}  /* namespace mw */

#endif /* EXCEPTION_H_ */
