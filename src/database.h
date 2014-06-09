#ifndef DATABASE_H
#define DATABASE_H

#include <QStandardItem>
#include <QDomDocument>

#include "jotter.h"
#include "tasker.h"

class DataBase
{
public:
  DataBase(Jotter *newJotter, QStandardItem *jotterRoot, Tasker *newTasker, QStandardItem *taskerRoot);
  QStandardItem *getRootJotterItem();
  QStandardItem *getRootTaskerItem();
  int write(QString filePath);
  int read(QString filePath);

private:
  void updateDOMKsilit();
  void updateDOMJotterHeader();
  void updateDOMJotterBody(QStandardItem *item);
  void updateDOMTaskerHeader();
  void updateDOMTaskerBody(QStandardItem *item);
  int readJotterHeader();
  int readTaskerHeader();
  int readJotterBody();
  int readTaskerBody();
  QDomNode getNextNode(QDomNode &currentNode, QDomNode &lastNode);
  QStandardItem *getItemById(QStandardItem *currentItem, int itemId);
  bool isDBInitialised();

  QStandardItem *rootJotterItem;
  QStandardItem *rootTaskerItem;
  Jotter *jotter;
  Tasker *tasker;
  QDomDocument domDoc;
};

#endif // DATABASE_H
