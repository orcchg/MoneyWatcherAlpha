/*
 * logger.cc
 *
 *  Description: Logger functions implementation.
 *
 *  Created on: 02.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "logger.h"


namespace mw {

Logger::Logger() {
  initialize();
}

Logger::~Logger() {
  this->deinitialize();
}

void Logger::initialize() {
  google::InitGoogleLogging(nullptr);
}

void Logger::deinitialize() {
  google::ShutdownGoogleLogging();
}

}  /* namespace mw */
