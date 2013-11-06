/*
 * engine.h
 *
 *  Description: Main MoneyWatcher engine declaration.
 *
 *  Created on: Nov 5, 2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <QtWidgets/QApplication>


#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();

private:
    void createMenu();
    void createHorizontalGroupBox();
    void createGridGroupBox();
    void createFormGroupBox();

    enum { NumGridRows = 3, NumButtons = 4 };

    QMenuBar *menuBar;
    QGroupBox *horizontalGroupBox;
    QGroupBox *gridGroupBox;
    QGroupBox *formGroupBox;
    QTextEdit *smallEditor;
    QTextEdit *bigEditor;
    QLabel *labels[NumGridRows];
    QLineEdit *lineEdits[NumGridRows];
    QPushButton *buttons[NumButtons];
    QDialogButtonBox *buttonBox;

    QMenu *fileMenu;
    QAction *exitAction;
};
//! [0]


namespace mw {

/// @class MoneyWatcherEngine
class MoneyWatcherEngine {
public:
  MoneyWatcherEngine(int argc, char* argv[]);
  virtual ~MoneyWatcherEngine();

  /// @brief Run Graphic application.
  void run();

private:
  QApplication m_application;

  MoneyWatcherEngine(const MoneyWatcherEngine& obj) = delete;
  MoneyWatcherEngine(MoneyWatcherEngine&& rval_obj) = delete;
  MoneyWatcherEngine& operator = (const MoneyWatcherEngine& rhs) = delete;
  MoneyWatcherEngine& operator = (MoneyWatcherEngine&& rval_rhs) = delete;
};

}  /* namespace mw */

#endif /* ENGINE_H_ */
