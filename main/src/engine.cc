/*
 * engine.cc
 *
 *  Description: Main MoneyWatcher engine implementation.
 *
 *  Created on: Nov 5, 2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#include "engine.h"
#include "logger.h"


#include <QtWidgets/QtWidgets>

//! [0]
Dialog::Dialog()
{
    createMenu();
    createHorizontalGroupBox();
    createGridGroupBox();
    createFormGroupBox();
//! [0]

//! [1]
    bigEditor = new QTextEdit;
    bigEditor->setPlainText(tr("This widget takes up all the remaining space "
                               "in the top-level layout."));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
//! [1]

//! [2]
    QVBoxLayout *mainLayout = new QVBoxLayout;
//! [2] //! [3]
    mainLayout->setMenuBar(menuBar);
//! [3] //! [4]
    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(gridGroupBox);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(bigEditor);
    mainLayout->addWidget(buttonBox);
//! [4] //! [5]
    setLayout(mainLayout);

    setWindowTitle(tr("Basic Layouts"));
}
//! [5]

//! [6]
void Dialog::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
}
//! [6]

//! [7]
void Dialog::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Horizontal layout"));
    QHBoxLayout *layout = new QHBoxLayout;

    for (int i = 0; i < NumButtons; ++i) {
        buttons[i] = new QPushButton(tr("Button %1").arg(i + 1));
  layout->addWidget(buttons[i]);
    }
    horizontalGroupBox->setLayout(layout);
}
//! [7]

//! [8]
void Dialog::createGridGroupBox()
{
    gridGroupBox = new QGroupBox(tr("Grid layout"));
//! [8]
    QGridLayout *layout = new QGridLayout;

//! [9]
    for (int i = 0; i < NumGridRows; ++i) {
        labels[i] = new QLabel(tr("Line %1:").arg(i + 1));
        lineEdits[i] = new QLineEdit;
        layout->addWidget(labels[i], i + 1, 0);
        layout->addWidget(lineEdits[i], i + 1, 1);
    }

//! [9] //! [10]
    smallEditor = new QTextEdit;
    smallEditor->setPlainText(tr("This widget takes up about two thirds of the "
                                 "grid layout."));
    layout->addWidget(smallEditor, 0, 2, 4, 1);
//! [10]

//! [11]
    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);
    gridGroupBox->setLayout(layout);
}
//! [11]

//! [12]
void Dialog::createFormGroupBox()
{
    formGroupBox = new QGroupBox(tr("Form layout"));
    QFormLayout *layout = new QFormLayout;
    layout->addRow(new QLabel(tr("Line 1:")), new QLineEdit);
    layout->addRow(new QLabel(tr("Line 2, long text:")), new QComboBox);
    layout->addRow(new QLabel(tr("Line 3:")), new QSpinBox);
    formGroupBox->setLayout(layout);
}
//! [12]

namespace mw {

MoneyWatcherEngine::MoneyWatcherEngine(int argc, char* argv[])
  : m_application(argc, argv) {
}

MoneyWatcherEngine::~MoneyWatcherEngine() {
}

void MoneyWatcherEngine::run() {
  INF("enter MoneyWatcherEngine::run().");
  //
  INF("exit MoneyWatcherEngine::run().");
}

}  /* namespace mw */
