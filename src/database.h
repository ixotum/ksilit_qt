#ifndef DATABASE_H
#define DATABASE_H

#include <QStandardItem>
#include <QDomDocument>

#include "jotter.h"

class DataBase
{
public:
  DataBase();
  void setRootItem(QStandardItem *newRootItem);
  QStandardItem *getRootItem();
  void setJotter(Jotter *newJotter);
  int write(QString filePath);
  int read(QString filePath);

private:
  void updateDOMKsilit();
  void updateDOMJotterHeader();
  void updateDOMJotterBody(QStandardItem *item);
  int readJotterHeader();
  QDomNode getNextNode(QDomNode &currentNode, QDomNode &lastNode);
  QStandardItem *getItemById(QStandardItem *currentItem, int itemId);

  QStandardItem *rootItem;
  Jotter *jotter;
  QDomDocument domDoc;
};

#endif // DATABASE_H
