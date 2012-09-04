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
