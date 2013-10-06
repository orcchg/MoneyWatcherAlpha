/*
 * main.cc
 *
 *  Description:
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include <iostream>
#include <locale.h>
#include "cycle_table.h"
#include "daily_table.h"


int main(int argc, char** argv) {
  setlocale(LC_CTYPE, "");
  std::cout << "Hello, World!" << std::endl;
  mw::DailyTable daily_table;
  return (0);
}
