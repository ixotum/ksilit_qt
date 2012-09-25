/*******************************************************************************
  FILE: ksilitunittest.cpp
  AUTOR: ixotum
  DATE: 2012-08-19 15:19:56
  DESCRIPTION: Implementation of KsilitUnitTest class corresponded for unit
    testing
*******************************************************************************/

#include <QtTest/QtTest>
#include <QModelIndex>
#include <QtXml/QDomDocument>
#include <QMessageBox>

#include "ksilitunittest.h"
#include "jot.h"
#include "jottermodel.h"
#include "defines.h"

KsilitUnitTest::KsilitUnitTest()
{
}

void KsilitUnitTest::jot_parent() {
  Jot *jot = new Jot(0);
  Jot *parent = jot->parent();
  QVERIFY(!parent);

  Jot *jot2 = new Jot(1, jot);
  Jot *parent2 = jot2->parent();
  QVERIFY(parent2);
}

void KsilitUnitTest::jot_insertChildren() {
  int childCount = 0;
  bool success = false;
  Jot *jot = new Jot(0);

  childCount = jot->childCount();
  QCOMPARE(childCount, 0);

  success = jot->insertChildren(1, 0, 1);
  QCOMPARE(success, true);

  childCount = jot->childCount();
  QCOMPARE(childCount, 1);

  Jot *childJot = jot->child(0);
  QVERIFY(childJot);
}

void KsilitUnitTest::jot_insertColumn() {
  int columnCount = 0;
  bool success = false;
  Jot *jot = new Jot(0);

  columnCount = jot->columnCount();
  QCOMPARE(columnCount, 1);

  success = jot->insertColumn(0, 1);
  QCOMPARE(success, true);

  columnCount = jot->columnCount();
  QCOMPARE(columnCount, 2);
}

void KsilitUnitTest::jot_setColumnData() {
  bool success = false;
  QVariant columnData = 0;
  Jot *jot = new Jot(0);

  columnData = jot->getColumnData(0);
  QCOMPARE(columnData.toInt(), 0);

  columnData = 1;
  success = jot->setColumnData(0, columnData);
  QCOMPARE(success, true);

  columnData = jot->getColumnData(0);
  QCOMPARE(columnData.toInt(), 1);
}

void KsilitUnitTest::jot_getId() {
  Jot *parentJot = new Jot(0);
  int id = parentJot->getId();
  QCOMPARE(id, 0);

  bool success = parentJot->insertChildren(1, 0, 1);
  QVERIFY(success);

  Jot *childJot = parentJot->child(0);
  id = childJot->getId();
  QCOMPARE(id, 1);
}

void KsilitUnitTest::jotterModel_insertRows() {
  QModelIndex indexParent;
  JotterModel *model = new JotterModel;
  bool success = false;

  int rowNumber = 0;
  rowNumber = model->rowCount(indexParent);
  QCOMPARE(rowNumber, 0);

  success = model->insertRow(0, indexParent);
  QCOMPARE(success, true);

  rowNumber = model->rowCount(indexParent);
  QCOMPARE(rowNumber, 1);
}

void KsilitUnitTest::jotterModel_insertColumns() {
  QModelIndex indexParent;
  JotterModel *model = new JotterModel;
  bool success = false;

  int columnNumber = model->columnCount(indexParent);
  QCOMPARE(columnNumber, 1);

  success = model->insertColumn(0);
  QCOMPARE(success, true);

  columnNumber = model->columnCount(indexParent);
  QCOMPARE(columnNumber, 2);
}

void KsilitUnitTest::jotterModel_setData() {
  bool success = false;
  QModelIndex parentIndex;
  QModelIndex childIndex;
  JotterModel *model = new JotterModel;

  success = model->insertRow(0, parentIndex);
  QCOMPARE(success, true);

  childIndex = model->index(0, 0, parentIndex);
  QVERIFY(childIndex.isValid());

  QVariant inputValue = "1";
  success = model->setData(childIndex, inputValue, Qt::EditRole);
  QCOMPARE(success, true);

  QVariant outputValue = model->data(childIndex, Qt::DisplayRole);
  QCOMPARE(outputValue.toInt(), 1);
}

void KsilitUnitTest::jotterModel_parent() {
  QModelIndex rootIndex;
  JotterModel *model = new JotterModel;
  bool success = false;

  success = model->insertRow(0, rootIndex);
  QCOMPARE(success, true);

  QModelIndex parentInputIndex = model->index(0, 0, rootIndex);
  QVERIFY(parentInputIndex.isValid());

  success = model->insertRow(0, parentInputIndex);
  QCOMPARE(success, true);

  QModelIndex childIndex = model->index(0, 0, parentInputIndex);
  QVERIFY(childIndex.isValid());

  QModelIndex parentOutputIndex = model->parent(childIndex);
  QVERIFY(parentInputIndex == parentOutputIndex);
}

void KsilitUnitTest::jotterModel_setHeader() {
  JotterModel *model = new JotterModel;
  QVariant headerInput = "header_data";
  bool success = false;

  success = model->setHeaderData(0, Qt::Horizontal, headerInput, Qt::EditRole);
  QCOMPARE(success, true);

  QVariant headerOutput = model->headerData(0, Qt::Horizontal, Qt::DisplayRole);
  QVERIFY(headerInput == headerOutput);
}

void KsilitUnitTest::jotterModel_selfTestDomDocument() {
  bool success = false;
  JotterModel *model = new JotterModel;
  QModelIndex parentIndex;
  success = model->insertRow(0, parentIndex);
  QCOMPARE(success, true);

  QModelIndex childIndex;
  QVERIFY(!childIndex.isValid());

  childIndex = model->index(0, 0, parentIndex);
  QVERIFY(childIndex.isValid());

  QVariant valueInput = "1";
  success = model->setData(childIndex, valueInput, Qt::EditRole);
  QCOMPARE(success, true);

  QVariant valueOutput = model->data(childIndex, Qt::DisplayRole);
  QCOMPARE(valueInput.toString(), valueOutput.toString());

  QDomDocument *domDocOut = model->getDomDocument();
  QDomElement domElementOut = domDocOut->firstChildElement();
  QVERIFY(domElementOut.isNull() != true);

  QString xmlStringOut = domDocOut->toString(2);
  QVERIFY(!xmlStringOut.isEmpty());

  QString fileName = "test_file";
  QFile fileOut;
  fileOut.setFileName(fileName);
  success = fileOut.open(QIODevice::WriteOnly);
  QVERIFY(success);

  QTextStream out;
  out.setDevice(&fileOut);
  out << xmlStringOut;
  fileOut.close();

  QFile fileIn;
  fileIn.setFileName(fileName);
  success = fileIn.open(QIODevice::ReadOnly);
  QVERIFY(success);

  QTextStream in;
  in.setDevice(&fileIn);
  QDomDocument domDocIn("myDocIn");
  QString errorMsg;
  int errorLine = 0;
  int errorColumn = 0;
  success = domDocIn.setContent(&fileIn, &errorMsg, &errorLine, &errorColumn);
  QVERIFY(success);
  fileIn.close();
  fileIn.remove();

  QDomElement domElementIn = domDocIn.documentElement();
  QVERIFY(domElementIn.isNull() != true);

  QString rootTag = domElementIn.tagName();
  QString referenceRootTag = KSILIT_JOTTER_DOM_ELEMENT_TAG;
  referenceRootTag += "0";
  QCOMPARE(rootTag, referenceRootTag);
}
