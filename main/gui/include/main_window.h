/*
 * main_window.h
 *
 *  Description: Declares main window class of Qt application.
 *
 *  Created on: Dec 15, 2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QtWidgets/QLayout>
#include <QtWidgets/QMainWindow>


namespace mw {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr, const char* name = "");
  //
  //
  virtual ~MainWindow();

protected:
  void closeEvent(QCloseEvent* event);
  void contextMenuEvent(QContextMenuEvent* event);

private slots:
  void addEntry();

private:
  QVBoxLayout* m_top_layout;
  QGridLayout* m_grid_layout;
};

}  /* namespace mw */

#endif /* MAIN_WINDOW_H_ */
