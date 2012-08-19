/*******************************************************************************
  FILE: main.cpp
  AUTOR: ixotum
  DATE: 2012-08-18 14:25:59
  DESCRIPTION: This file contains main function
*******************************************************************************/

#define MAIN_DBG_LVL     0
#define MAIN_DBG_OPTIONS DBG_W+DBG_E//+DBG_S+DBG_R

#define MODULE_DBG_LVL     MAIN_DBG_LVL
#define MODULE_DBG_OPTIONS MAIN_DBG_OPTIONS
#include "dbg.h"

#include <QtGui/QApplication>
#include <QtTest/QtTest>

#include "mainwindow.h"
#include "ksilitunittest.h"

/*******************************************************************************
  NAME: unitTestProcess
  DESCRIPTION: Function for launching unit tests mechanism
  ARGUMENTS:
  Input:
    void
  Output:
    int
  RETURN VALUE:
    0 - if successful
    another value - tests fail
*******************************************************************************/
int unitTestProcess() {
  int rv = QTest::qExec(new KsilitUnitTest);

  return rv;
}

/*******************************************************************************
  NAME: main
  DESCRIPTION: The main function.
  ARGUMENTS:
  Input:
    int argc
    char *argv[]
  Output:
    int
  RETURN VALUE:
    0 - if successful
*******************************************************************************/
int main(int argc, char *argv[]) {
  DBGS(PRINT_START("void"));

  int rv = 0;

  rv = unitTestProcess();

  if (rv == 0) {
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    a.exec();
  }
  else {
    DBGE(PRINT_ERROR("Unit stets fail!"));
  }

  DBGR(PRINT_RETURN("ksilit terminated."));
  return rv;
}
