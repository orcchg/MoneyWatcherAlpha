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
#include <QApplication>


int main(int argc, char** argv) {
  setlocale(LC_CTYPE, "");
  std::cout << "Hello, World!" << std::endl;
  return (0);
}
