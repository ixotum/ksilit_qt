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
#include <QDebug>
#include <QDate>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  DBGS(PRINT_START("void"));

  QApplication a(argc, argv);
  MainWindow w;

  w.show();
  a.exec();

  DBGR(PRINT_RETURN("ksilit terminated."));
  return 0;
}
