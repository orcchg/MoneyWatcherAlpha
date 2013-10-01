/*
 * singleton.h
 *
 *  Description: Singleton OOP pattern for C++11.
 *
 *  Created on: 02.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_


template<class InstanceClass>
class Singleton {
public:
  Singleton(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator = (const Singleton&) = delete;
  Singleton&& operator = (Singleton&&) = delete;
  static InstanceClass &instance() {
    static InstanceClass inst;
    return (inst);
  }
  virtual ~Singleton() {}

protected:
#if __GNUC__ == 4 && __GNUC_MINOR__ < 6
  Singleton() {}
#else
  Singleton() = default;
#endif
};

#endif /* SINGLETON_H_ */
