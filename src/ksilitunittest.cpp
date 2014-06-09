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
  QStandardItem *rootJotterItem = new QStandardItem();
  Jotter *jotter = new Jotter();
  QStandardItem *rootTaskerItem = new QStandardItem();
  Tasker *tasker = new Tasker();

  DataBase dataBaseWrite(jotter, rootJotterItem, tasker, rootTaskerItem);

  //Add jotter item1 to the root jotter item
  QStandardItem *jotterItem1 = new QStandardItem();
  rootJotterItem->appendRow(jotterItem1);

  int jotterId1 = jotter->createJot();
  QVariant jotterItemData1 = jotterId1;
  jotterItem1->setData(jotterItemData1);

  QString jotterItemName1 = "name1";
  jotter->setName(jotterId1, jotterItemName1);

  //Add jotter item2 to the root jotter item
  QStandardItem *jotterItem2 = new QStandardItem();
  rootJotterItem->appendRow(jotterItem2);

  int jotterId2 = jotter->createJot();
  QVariant jotterItemData2 = jotterId2;
  jotterItem2->setData(jotterItemData2);

  QString jotterItemName2 = "name2";
  jotter->setName(jotterId2, jotterItemName2);

  //Add jotter item3 to the jotter item1
  QStandardItem *jotterItem3 = new QStandardItem();
  jotterItem1->appendRow(jotterItem3);

  int jotterId3 = jotter->createJot();
  QVariant jotterItemData3 = jotterId3;
  jotterItem3->setData(jotterItemData3);

  QString jotterItemName3 = "name3";
  jotter->setName(jotterId3, jotterItemName3);

  //Add tasker item1 to the root tasker item
  QStandardItem *taskerItem1 = new QStandardItem();
  rootTaskerItem->appendRow(taskerItem1);

  int taskerId1 = tasker->createTask();
  QVariant taskerItemData1 = taskerId1;
  taskerItem1->setData(taskerItemData1);

  QString taskerItemName1 = "name1";
  tasker->setName(taskerId1, taskerItemName1);

  //Add tasker item2 to the root tasker item
  QStandardItem *taskerItem2 = new QStandardItem();
  rootTaskerItem->appendRow(taskerItem2);

  int taskerId2 = tasker->createTask();
  QVariant taskerItemData2 = taskerId2;
  taskerItem2->setData(taskerItemData2);

  QString taskerItemName2 = "name2";
  tasker->setName(taskerId2, taskerItemName2);

  //Add tasker item3 to the tasker item1
  QStandardItem *taskerItem3 = new QStandardItem();
  taskerItem1->appendRow(taskerItem3);

  int taskerId3 = tasker->createTask();
  QVariant taskerItemData3 = taskerId3;
  taskerItem3->setData(taskerItemData3);

  QString taskerItemName3 = "name3";
  tasker->setName(taskerId3, taskerItemName3);

  QString fileName = "test file";
  int rv = dataBaseWrite.write(fileName);
  QCOMPARE(rv, 0);
}

void KsilitUnitTest::database_test_read() {
  QStandardItem *rootJotterItem = new QStandardItem();
  Jotter *jotter = new Jotter();
  QStandardItem *rootTaskerItem = new QStandardItem();
  Tasker *tasker = new Tasker();

  DataBase dataBaseRead(jotter, rootJotterItem, tasker, rootTaskerItem);
  QString fileName = "test file";
  int rv = dataBaseRead.read(fileName);
  QCOMPARE(rv, 0);

  QFile file(fileName);
  file.remove();

  QStandardItem *rootJotterItemRead = dataBaseRead.getRootJotterItem();
  QVERIFY(rootJotterItemRead);

  QStandardItem *jotterItem1 = rootJotterItemRead->child(0);
  QString jotterItemName1 = jotterItem1->text();
  QCOMPARE(jotterItemName1, QString("name1"));
  int jotterId1 = jotterItem1->data().toInt();
  QCOMPARE(jotterId1, 1);

  QStandardItem *jotterItem2 = rootJotterItemRead->child(1);
  QString jotterItemName2 = jotterItem2->text();
  QCOMPARE(jotterItemName2, QString("name2"));
  int jotterId2 = jotterItem2->data().toInt();
  QCOMPARE(jotterId2, 2);

  QStandardItem *jotterItem3 = jotterItem1->child(0);
  QString jotterItemName3 = jotterItem3->text();
  QCOMPARE(jotterItemName3, QString("name3"));
  int jotterId3 = jotterItem3->data().toInt();
  QCOMPARE(jotterId3, 3);

  QStandardItem *rootTaskerItemRead = dataBaseRead.getRootTaskerItem();
  QVERIFY(rootTaskerItemRead);

  int taskChildCount = rootTaskerItemRead->rowCount();
  QCOMPARE(taskChildCount, 2);

  QStandardItem *taskerItem1 = rootTaskerItemRead->child(0);
  QString taskerItemName1 = taskerItem1->text();
  QCOMPARE(taskerItemName1, QString("name1"));
  //Add checking for another task items
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
