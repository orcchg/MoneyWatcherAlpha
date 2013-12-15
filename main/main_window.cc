/*
 * main_widnow.cc
 *
 *  Description: Main GUI application implementation.
 *
 *  Created on: Dec 15, 2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "include/main_window.h"


namespace mw {

MainWindow::MainWindow(QWidget* parent, const char* name)
  : m_top_layout(new QVBoxLayout(this))
  , m_grid_layout(new QGridLayout()) {
  this->m_top_layout->addLayout(this->m_top_layout);
}

MainWindow::~MainWindow() {
  if (this->m_top_layout) {
    delete this->m_top_layout;
  }
  this->m_top_layout = nullptr;
  if (this->m_grid_layout) {
    delete this->m_grid_layout;
  }
  this->m_grid_layout = nullptr;
}

/* Protected */
void MainWindow::closeEvent(QCloseEvent* event) {

}

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {

}

/* Private slots */
void MainWindow::addEntry() {

}

/* Private */

}  /* namespace mw */
