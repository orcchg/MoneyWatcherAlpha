/*
 * serializable.h
 *
 *  Description: Serializable interface declaration.
 *
 *  Created on: 26.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <string>


namespace mw {

class iSerializable {
public:
  virtual ~iSerializable() {}

  virtual void serialize(std::string* output) const = 0;
};

}  /* namespace mw */

#endif /* SERIALIZABLE_H_ */
