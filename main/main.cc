/*
 * main.cc
 *
 *  Description:
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "engine.h"


int main(int argc, char** argv) {

  QApplication app(argc, argv);
  Dialog dialog;
  dialog.show();

  return (app.exec());

  return (0);
}
