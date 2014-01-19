/*
 * main.cc
 *
 *  Description: Main console application implementation.
 *
 *  Created on: 19.01.2014
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "src/engine.h"


int main(int argc, char** argv) {

  mw::ConsoleEngine engine;
  engine.run();

  return 0;
}
