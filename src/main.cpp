/*******************************************************************************
  FILE: main.cpp
  AUTOR: ixotum
  DATE: 2012-08-18 14:25:59
  DESCRIPTION: This file contains main function
*******************************************************************************/

#include <QtGui/QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;

  w.show();
  a.exec();

  return 0;
}
