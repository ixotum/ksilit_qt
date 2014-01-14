#include <QtTest/QtTest>
#include <QStandardItem>

#include "ksilitunittest.h"
#include "jot.h"
#include "jotter.h"
#include "database.h"
#include "task.h"
#include "tasker.h"

KsilitUnitTest::KsilitUnitTest()
{
}

void KsilitUnitTest::jotter_test_jotCounter() {
  Jotter jotter;
  int jotNumber = jotter.getJotCount();
  QCOMPARE(jotNumber, 0);

  int rv = jotter.setJotCount(1);
  QCOMPARE(rv, 0);

  int id = jotter.getJotCount();
  QCOMPARE(id, 1);
}

void KsilitUnitTest::jot_test_name_field() {
  Jot *jot = new Jot();
  QVERIFY(jot);

  QString name = jot->getName();
  QVERIFY(name.isEmpty());

  QString setName = "test name";
  jot->setName(setName);
  QString getName = jot->getName();
  QCOMPARE(setName, getName);
}

void KsilitUnitTest::jot_test_text() {
  Jot *jot = new Jot();
  QString text = jot->getText();
  QVERIFY(text.isEmpty());

  text = "test text";
  jot->setText(text);
  QString getText = jot->getText();
  QCOMPARE(text, getText);
}

void KsilitUnitTest::jotter_test_jot_creating() {
  Jotter *jotter = new Jotter();
  int id = jotter->createJot();
  QCOMPARE(id, 1);

  QString name = jotter->getName(id);
  QVERIFY(name.isEmpty());

  QString setName = "test name";
  int rv = jotter->setName(id, setName);
  QCOMPARE(rv, 0);

  QString getName = jotter->getName(id);
  QCOMPARE(setName, getName);
}

void KsilitUnitTest::database_test_write() {
  DataBase dataBaseWrite;
  QStandardItem *rootItem = new QStandardItem();
  Jotter *jotter = new Jotter();

  dataBaseWrite.setRootItem(rootItem);
  dataBaseWrite.setJotter(jotter);

  //Add item1 to the root item
  QStandardItem *item1 = new QStandardItem();
  rootItem->appendRow(item1);

  int id1 = jotter->createJot();
  QVariant itemData1 = id1;
  item1->setData(itemData1);

  QString itemName1 = "name1";
  jotter->setName(id1, itemName1);

  //Add item2 to the root item
  QStandardItem *item2 = new QStandardItem();
  rootItem->appendRow(item2);

  int id2 = jotter->createJot();
  QVariant itemData2 = id2;
  item2->setData(itemData2);

  QString itemName2 = "name2";
  jotter->setName(id2, itemName2);

  //Add item3 to the root item1
  QStandardItem *item3 = new QStandardItem();
  item1->appendRow(item3);

  int id3 = jotter->createJot();
  QVariant itemData3 = id3;
  item3->setData(itemData3);

  QString itemName3 = "name3";
  jotter->setName(id3, itemName3);

  QString fileName = "test file";
  int rv = dataBaseWrite.write(fileName);
  QCOMPARE(rv, 0);
}

void KsilitUnitTest::database_test_read() {
  DataBase dataBaseRead;
  QString fileName = "test file";
  int rv = dataBaseRead.read(fileName);
  QCOMPARE(rv, 0);

  QFile file(fileName);
  file.remove();

  QStandardItem *rootItem = dataBaseRead.getRootItem();
  QVERIFY(rootItem);

  QStandardItem *item1 = rootItem->child(0);
  QString itemName1 = item1->text();
  QCOMPARE(itemName1, QString("name1"));
  int id1 = item1->data().toInt();
  QCOMPARE(id1, 1);

  QStandardItem *item2 = rootItem->child(1);
  QString itemName2 = item2->text();
  QCOMPARE(itemName2, QString("name2"));
  int id2 = item2->data().toInt();
  QCOMPARE(id2, 2);

  QStandardItem *item3 = item1->child(0);
  QString itemName3 = item3->text();
  QCOMPARE(itemName3, QString("name3"));
  int id3 = item3->data().toInt();
  QCOMPARE(id3, 3);
}

void KsilitUnitTest::jotter_test_text() {
  QString setText = "test text";
  Jotter jotter;
  int id = jotter.createJot();
  int rv = jotter.setText(id, setText);
  QCOMPARE(rv, 0);

  QString getText = jotter.getText(id);
  QCOMPARE(setText, getText);
}

void KsilitUnitTest::task_test_name() {
  Task *task = new Task();
  QVERIFY(task);

  QString name = task->getName();
  QVERIFY(name.isEmpty());

  QString setName = "test name";
  task->setName(setName);
  QString getName = task->getName();
  QCOMPARE(setName, getName);
}

void KsilitUnitTest::tasker_test_task_creating() {
  Tasker *tasker = new Tasker();
  int id = tasker->createTask();
  QCOMPARE(id, 1);

  QString name = tasker->getName(id);
  QVERIFY(name.isEmpty());

  QString setName = "test name";
  int rv = tasker->setName(id, setName);
  QCOMPARE(rv, 0);

  QString getName = tasker->getName(id);
  QCOMPARE(setName, getName);
}
