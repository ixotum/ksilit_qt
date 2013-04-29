#define MAIN_DBG_LVL 0
#define MAIN_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL MAIN_DBG_LVL
#define MODULE_DBG_OPTIONS MAIN_DBG_OPTIONS
#include "dbg.h"

#include <QtTest/QtTest>
#include <QtGui/QApplication>

#include "ksilitunittest.h"
#include "mainwindow.h"

int unitTestProcess() {
  int rv = QTest::qExec(new KsilitUnitTest);

  return rv;
}

int main(int argc, char *argv[]) {
  DBGS(PRINT_START());

  int rv = 0;

  rv = unitTestProcess();

  if (rv == 0) {
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    a.exec();
  }
  else {
    DBGE(PRINT_ERROR("Unit tests fail!"));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}
