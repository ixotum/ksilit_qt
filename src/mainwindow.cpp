/*******************************************************************************
  FILE: mainwindow.cpp
  AUTOR: ixotum
  DATE: 2012-08-18 14:27:37
  DESCRIPTION: This file contains implementation of MainWindow class
*******************************************************************************/

#define MAINWINDOW_DBG_LVL     0
#define MAINWINDOW_DBG_OPTIONS DBG_W+DBG_E//+DBG_S+DBG_R

#define MODULE_DBG_LVL     MAINWINDOW_DBG_LVL
#define MODULE_DBG_OPTIONS MAINWINDOW_DBG_OPTIONS
#include "dbg.h"

#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  createConnections();
}

MainWindow::~MainWindow()
{
  delete ui;
}

/*******************************************************************************
  NAME: createConnections
  DESCRIPTION: Creating connections to the slots
  ARGUMENTS:
  Input:
    void
  Output:
    void
  RETURN VALUE:
    void
*******************************************************************************/
void MainWindow::createConnections() {
  DBGS(PRINT_START("void"));

  connect(ui->action_About, SIGNAL(triggered()), this, SLOT(ksilitSlotHelpAbout()));

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: ksilitSlotHelpAbout
  DESCRIPTION: Showing Help->About message
  ARGUMENTS:
  Input:
    void
  Output:
    void
  RETURN VALUE:
    void
*******************************************************************************/
void MainWindow::ksilitSlotHelpAbout() {
  DBGS(PRINT_START("void"));

  QMessageBox mBox;

  mBox.setText(QString("ksilit: ") + KSILIT_VERSION);
  mBox.exec();

  DBGR(PRINT_RETURN());
}
