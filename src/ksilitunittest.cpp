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

void KsilitUnitTest::jotColumnCount() {
  int columnCount = 0;
  Jot *jot = new Jot;

  columnCount = jot->columnCount();
  QCOMPARE(columnCount, 1);
}
