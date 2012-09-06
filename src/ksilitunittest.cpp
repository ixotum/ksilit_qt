/*******************************************************************************
  FILE: ksilitunittest.cpp
  AUTOR: ixotum
  DATE: 2012-08-19 15:19:56
  DESCRIPTION: Implementation of KsilitUnitTest class corresponded for unit
    testing
*******************************************************************************/

#include <QtTest/QtTest>

#include "ksilitunittest.h"
#include "jot.h"

KsilitUnitTest::KsilitUnitTest()
{
}

void KsilitUnitTest::jotParent() {
  Jot *jot = new Jot;
  Jot *parent = jot->parent();
  QVERIFY(!parent);

  Jot *jot2 = new Jot(jot);
  Jot *parent2 = jot2->parent();
  QVERIFY(parent2);
}

void KsilitUnitTest::jotInsertChildren() {
  int childCount = 0;
  bool success = false;
  Jot *jot = new Jot;

  childCount = jot->childCount();
  QCOMPARE(childCount, 0);

  success = jot->insertChildren(0, 1);
  QCOMPARE(success, true);

  childCount = jot->childCount();
  QCOMPARE(childCount, 1);
}

void KsilitUnitTest::jotInsertColumn() {
  int columnCount = 0;
  bool success = false;
  Jot *jot = new Jot;

  columnCount = jot->columnCount();
  QCOMPARE(columnCount, 1);

  success = jot->insertColumn(0, 1);
  QCOMPARE(success, true);

  columnCount = jot->columnCount();
  QCOMPARE(columnCount, 2);
}

void KsilitUnitTest::jotSetColumnData() {
  bool success = false;
  QVariant columnData = 0;
  Jot *jot = new Jot;

  columnData = jot->getColumnData(0);
  QCOMPARE(columnData.toInt(), 0);

  columnData = 1;
  success = jot->setColumnData(0, columnData);
  QCOMPARE(success, true);

  columnData = jot->getColumnData(0);
  QCOMPARE(columnData.toInt(), 1);
}
