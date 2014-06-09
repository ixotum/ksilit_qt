#define DATABASE_DBG_LVL 3
#define DATABASE_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL DATABASE_DBG_LVL
#define MODULE_DBG_OPTIONS DATABASE_DBG_OPTIONS
#include "dbg.h"

#include <QFile>
#include <QTextStream>

#include "database.h"
#include "defines.h"

DataBase::DataBase(Jotter *newJotter, QStandardItem *jotterRoot, Tasker *newTasker, QStandardItem *taskerRoot)
  : rootJotterItem(jotterRoot)
  , rootTaskerItem(taskerRoot)
  , jotter(newJotter)
  , tasker(newTasker)
{
}

QStandardItem *DataBase::getRootJotterItem() {
  return rootJotterItem;
}

QStandardItem *DataBase::getRootTaskerItem() {
  return rootTaskerItem;
}

void DataBase::updateDOMKsilit() {
  DBGS(PRINT_START(""));

  domDoc.clear();

  QString ksilitName = KSILIT_DOM_TAG;
  QDomElement ksilitElement = domDoc.createElement(ksilitName);
  domDoc.appendChild(ksilitElement);

  DBGR(PRINT_RETURN(""));
}

void DataBase::updateDOMJotterHeader() {
  DBGS(PRINT_START(""));

  QString ksilitName = KSILIT_DOM_TAG;
  QDomNodeList ksilitNodeList = domDoc.elementsByTagName(ksilitName);
  QDomNode ksilitNode = ksilitNodeList.at(0);
  QDomElement ksilitElement = ksilitNode.toElement();

  QString headerName = KSILIT_JOTTER_DOM_HEADER_TAG;
  QDomElement headerElement = domDoc.createElement(headerName);
  QString attributeJotCounter = KSILIT_JOTTER_DOM_JOT_COUNTER;
  int jotCounter = jotter->getJotCount();
  headerElement.setAttribute(attributeJotCounter, jotCounter);
  ksilitElement.appendChild(headerElement);

  DBGR(PRINT_RETURN(""));
}

void DataBase::updateDOMJotterBody(QStandardItem *item) {
  if (item != rootJotterItem) {
    int parentId = 0;
    QStandardItem *parentItem = item->parent();

    if (parentItem) {
      QVariant parentData = parentItem->data();
      DBG3(PRINT_DBG("parentData: %s", qPrintable(parentData.toString())));

      if (!parentData.isNull()) {
        parentId = parentData.toInt();
      }
      DBG3(PRINT_DBG("parentId: %i", parentId));
    }

    //If parentId still equal zero, than parent of the item is invisible root
    QString parentTag = KSILIT_JOTTER_DOM_ELEMENT_TAG;
    parentTag += QString::number(parentId);
    QDomNodeList parentNodeList = domDoc.elementsByTagName(parentTag);

    if (!parentNodeList.isEmpty()) {
      if (parentNodeList.count() == 1) {
        QDomNode parentNode = parentNodeList.at(0);
        QDomElement parentElement = parentNode.toElement();

        if (!parentElement.isNull()) {
          QVariant childData = item->data();
          int childId = childData.toInt();
          QString childTag = KSILIT_JOTTER_DOM_ELEMENT_TAG;
          childTag += QString::number(childId);
          QDomElement childElement = domDoc.createElement(childTag);

          QString attributeChildId = QString::number(childId);
          DBG3(PRINT_DBG("attributeChildId: %s", qPrintable(attributeChildId)));
          childElement.setAttribute(KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_ID, attributeChildId);

          QString attributeChildName = jotter->getName(childId);
          DBG3(PRINT_DBG("attributeChildName: %s", qPrintable(attributeChildName)));
          childElement.setAttribute(KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_NAME, attributeChildName);

          QString attributeChildText = jotter->getText(childId);
          DBG3(PRINT_DBG("attributeChildText: %s", qPrintable(attributeChildText)));
          childElement.setAttribute(KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_TEXT, attributeChildText);

          parentElement.appendChild(childElement);
        }
        else {
          DBGE(PRINT_ERROR("Can't convert parentNode: %s", qPrintable(parentTag)));
        }
      }
      else {
        DBGE(PRINT_ERROR("Multiple difinition of parent tag: %s", qPrintable(parentTag)));
      }
    }
    else {
      DBGE(PRINT_ERROR("parentNodeList is empty! Parent tag: %s", qPrintable(parentTag)));
    }
  }
  else {//Current item is invisible root item
    QString ksilitName = KSILIT_DOM_TAG;
    QDomNodeList ksilitNodeList = domDoc.elementsByTagName(ksilitName);
    QDomNode ksilitNode = ksilitNodeList.at(0);
    QDomElement ksilitElement = ksilitNode.toElement();

    QString bodyName = KSILIT_JOTTER_DOM_BODY_TAG;
    QDomElement bodyElement = domDoc.createElement(bodyName);
    ksilitElement.appendChild(bodyElement);

    int rootId = 0;
    QString rootName = KSILIT_JOTTER_DOM_ELEMENT_TAG;
    rootName += QString::number(rootId);
    QDomElement rootElement = domDoc.createElement(rootName);
    bodyElement.appendChild(rootElement);
  }

  int childCount = item->rowCount();
  for (int i = 0; i < childCount; ++i) {
    QStandardItem *nextItem = item->child(i);
    updateDOMJotterBody(nextItem);
  }
}

void DataBase::updateDOMTaskerHeader() {
  DBGS(PRINT_START(""));

  QString ksilitName = KSILIT_DOM_TAG;
  QDomNodeList ksilitNodeList = domDoc.elementsByTagName(ksilitName);
  QDomNode ksilitNode = ksilitNodeList.at(0);
  QDomElement ksilitElement = ksilitNode.toElement();

  QString headerName = KSILIT_TASKER_DOM_HEADER_TAG;
  QDomElement headerElement = domDoc.createElement(headerName);
  QString attributeTaskCounter = KSILIT_TASKER_DOM_TASK_COUNTER;
  int taskCounter = tasker->getTaskCount();
  headerElement.setAttribute(attributeTaskCounter, taskCounter);
  ksilitElement.appendChild(headerElement);

  DBGR(PRINT_RETURN(""));
}

void DataBase::updateDOMTaskerBody(QStandardItem *item) {
  if (item != rootTaskerItem) {
    int parentId = 0;
    QStandardItem *parentItem = item->parent();

    if (parentItem) {
      QVariant parentData = parentItem->data();
      DBG3(PRINT_DBG("parentData: %s", qPrintable(parentData.toString())));

      if (!parentData.isNull()) {
        parentId = parentData.toInt();
      }
      DBG3(PRINT_DBG("parentId: %i", parentId));
    }

    //If parentId still equal zero, than parent of the item is invisible root
    QString parentTag = KSILIT_TASKER_DOM_ELEMENT_TAG;
    parentTag += QString::number(parentId);
    QDomNodeList parentNodeList = domDoc.elementsByTagName(parentTag);

    if (!parentNodeList.isEmpty()) {
      if (parentNodeList.count() == 1) {
        QDomNode parentNode = parentNodeList.at(0);
        QDomElement parentElement = parentNode.toElement();

        if (!parentElement.isNull()) {
          QVariant childData = item->data();
          int childId = childData.toInt();
          QString childTag = KSILIT_TASKER_DOM_ELEMENT_TAG;
          childTag += QString::number(childId);
          QDomElement childElement = domDoc.createElement(childTag);

          QString attributeChildId = QString::number(childId);
          DBG3(PRINT_DBG("attributeChildId: %s", qPrintable(attributeChildId)));
          childElement.setAttribute(KSILIT_TASKER_DOM_ELEMENT_ATTRIBUTE_ID, attributeChildId);

          QString attributeChildName = tasker->getName(childId);
          DBG3(PRINT_DBG("attributeChildName: %s", qPrintable(attributeChildName)));
          childElement.setAttribute(KSILIT_TASKER_DOM_ELEMENT_ATTRIBUTE_NAME, attributeChildName);

          QString attributeChildText = tasker->getText(childId);
          DBG3(PRINT_DBG("attributeChildText: %s", qPrintable(attributeChildText)));
          childElement.setAttribute(KSILIT_TASKER_DOM_ELEMENT_ATTRIBUTE_TEXT, attributeChildText);

          parentElement.appendChild(childElement);
        }
        else {
          DBGE(PRINT_ERROR("Can't convert parentNode: %s", qPrintable(parentTag)));
        }
      }
      else {
        DBGE(PRINT_ERROR("Multiple difinition of parent tag: %s", qPrintable(parentTag)));
      }
    }
    else {
      DBGE(PRINT_ERROR("parentNodeList is empty! Parent tag: %s", qPrintable(parentTag)));
    }
  }
  else {//Current item is invisible root item
    QString ksilitName = KSILIT_DOM_TAG;
    QDomNodeList ksilitNodeList = domDoc.elementsByTagName(ksilitName);
    QDomNode ksilitNode = ksilitNodeList.at(0);
    QDomElement ksilitElement = ksilitNode.toElement();

    QString bodyName = KSILIT_TASKER_DOM_BODY_TAG;
    QDomElement bodyElement = domDoc.createElement(bodyName);
    ksilitElement.appendChild(bodyElement);

    int rootId = 0;
    QString rootName = KSILIT_TASKER_DOM_ELEMENT_TAG;
    rootName += QString::number(rootId);
    QDomElement rootElement = domDoc.createElement(rootName);
    bodyElement.appendChild(rootElement);
  }

  int childCounf = item->rowCount();
  for (int i = 0; i < childCounf; ++i) {
    QStandardItem *nextItem = item->child(i);
    updateDOMTaskerBody(nextItem);
  }
}

bool DataBase::isDBInitialised() {
  if (rootJotterItem && rootTaskerItem && jotter && tasker) {
    return true;
  }
  return false;
}

int DataBase::write(QString filePath) {
  DBGS(PRINT_START("filePath: %s", qPrintable(filePath)));

  int rv = ERROR_UNKNOWN_ERROR;

  if (isDBInitialised()) {
    updateDOMKsilit();
    updateDOMJotterHeader();
    updateDOMJotterBody(rootJotterItem);
    updateDOMTaskerHeader();
    updateDOMTaskerBody(rootTaskerItem);

    QString xmlString = domDoc.toString();

    if (!xmlString.isEmpty()) {
      QFile fileOut(filePath);

      if (fileOut.open(QIODevice::WriteOnly)) {
        QTextStream outStream(&fileOut);
        outStream << xmlString;
        fileOut.close();
        rv = ALL_OK;
      }
      else {
        DBGE(PRINT_ERROR("Error opening file: %s", qPrintable(filePath)));
        rv = ERROR_OPENING_FILE;
      }
    }
    else {
      DBGE(PRINT_ERROR("xmlString is empty!"));
    }
  }
  else {
    rv = ERROR_DB_NOT_INITIALIZED;
    DBGE(PRINT_ERROR("Data base not initialized!"));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

QDomNode DataBase::getNextNode(QDomNode &currentNode, QDomNode &lastNode) {
  DBGS(PRINT_START("currentNode: 0x%08x, lastNode: 0x%08x", &currentNode, &lastNode));

  QDomNode nextNode;

  if (currentNode.hasChildNodes()) {
    nextNode = currentNode.firstChild();
  }
  else {
    QDomNode previousNode = currentNode;
    nextNode = currentNode.nextSibling();

    while (nextNode.isNull()) {
      QDomNode parentNode = previousNode.parentNode();

      if (parentNode == lastNode) {
        break;
      }
      else {
        previousNode = parentNode;
        nextNode = parentNode.nextSibling();
      }
    }
  }

  DBGR(PRINT_RETURN("nextNode: 0x%08x", &nextNode));
  return nextNode;
}

QStandardItem *DataBase::getItemById(QStandardItem *currentItem, int itemId) {
  QStandardItem *foundItem = 0;
  QVariant currentData = currentItem->data();
  int currentId = currentData.toInt();

  if (currentId != itemId) {
    int childCount = currentItem->rowCount();
    for (int i = 0; i < childCount; ++i) {
      foundItem = getItemById(currentItem->child(i), itemId);

      if (foundItem) {
        break;
      }
    }
  }
  else {
    foundItem = currentItem;
  }

  return foundItem;
}

int DataBase::readJotterHeader() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  QString headerName = KSILIT_JOTTER_DOM_HEADER_TAG;
  QDomNodeList headerList = domDoc.elementsByTagName(headerName);
  QDomNode headerNode = headerList.at(0);
  QDomElement headerElement = headerNode.toElement();
  QString attributeJotCounter = headerElement.attribute(KSILIT_JOTTER_DOM_JOT_COUNTER);
  int jotCounter = attributeJotCounter.toInt();
  rv = jotter->setJotCount(jotCounter);

  if (rv != ALL_OK) {
    DBGE(PRINT_ERROR("Error setting jotCounter!"));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

int DataBase::readTaskerHeader() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  QString headerName = KSILIT_TASKER_DOM_HEADER_TAG;
  QDomNodeList headerList = domDoc.elementsByTagName(headerName);
  QDomNode headerNode = headerList.at(0);
  QDomElement headerElement = headerNode.toElement();
  QString attributeTaskCounter = headerElement.attribute(KSILIT_TASKER_DOM_TASK_COUNTER);
  int taskCounter = attributeTaskCounter.toInt();
  rv = tasker->setTaskCount(taskCounter);

  if (rv != ALL_OK) {
    DBGE(PRINT_ERROR("Error setting taskCounter!"));
  }

  DBGR(PRINT_RETURN(""));
  return rv;
}

int DataBase::readJotterBody() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  QString bodyName = KSILIT_JOTTER_DOM_BODY_TAG;
  QDomNodeList bodyList = domDoc.elementsByTagName(bodyName);
  QDomNode bodyNode = bodyList.at(0);
  QDomElement bodyElement = bodyNode.toElement();

  if (!bodyElement.isNull()) {
    int rootId = 0;
    QString rootNodeName = KSILIT_JOTTER_DOM_ELEMENT_TAG;
    rootNodeName += QString::number(rootId);
    QDomNodeList rootNodeList = domDoc.elementsByTagName(rootNodeName);
    QDomNode rootNode = rootNodeList.at(0);
    QDomElement rootElement = rootNode.toElement();
    QDomNode childNode = rootElement.firstChild();

    while (!childNode.isNull()) {
      QDomElement childElement = childNode.toElement();
      QString childTag = childElement.tagName();
      DBG3(PRINT_DBG("childTag: %s", qPrintable(childTag)));

      QString attributeChildId = childElement.attribute(KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_ID);
      DBG3(PRINT_DBG("attributeChildId: %s", qPrintable(attributeChildId)));

      QString attributeChildName = childElement.attribute(KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_NAME);
      DBG3(PRINT_DBG("attributeChildName: %s", qPrintable(attributeChildName)));

      QString attributeChildText = childElement.attribute(KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_TEXT);
      DBG3(PRINT_DBG("attributeChildText: %s", qPrintable(attributeChildText)));

      QDomNode parentNode = childElement.parentNode();

      if (!parentNode.isNull()) {
        QDomElement parentElement = parentNode.toElement();

        if (!parentElement.isNull()) {
          int parentId = 0;
          QString attributeParentId = parentElement.attribute(KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_ID);

          if (!attributeParentId.isEmpty()) {
            parentId = attributeParentId.toInt();
          }

          DBG3(PRINT_DBG("parentId: %i", parentId));

          QStandardItem *parentItem = rootJotterItem;

          if (parentId) {
            parentItem = getItemById(rootJotterItem, parentId);
          }

          if (parentItem){
            QStandardItem *childItem = new QStandardItem();
            parentItem->appendRow(childItem);

            QVariant childData = attributeChildId;
            childItem->setData(childData);

            childItem->setText(attributeChildName);

            int childId = attributeChildId.toInt();
            jotter->createJot(childId);
            jotter->setName(childId, attributeChildName);
            jotter->setText(childId, attributeChildText);
          }
          else {
            DBGE(PRINT_ERROR("parentItem not found!"));
          }
        }
        else {
          DBGE(PRINT_ERROR("Error converting parent node to element!"));
        }
      }
      else {
        DBGE(PRINT_ERROR("parentNode is null!"));
      }

      childNode = getNextNode(childNode, rootNode);

      if (childNode.isNull()) {
        rv = ALL_OK;
      }
    }
  }
  else {
    DBGE(PRINT_ERROR("rootElement is null!"));
  }

  DBGR(PRINT_RETURN(""));
  return rv;
}

int DataBase::readTaskerBody() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  QString bodyName = KSILIT_TASKER_DOM_BODY_TAG;
  QDomNodeList bodyList = domDoc.elementsByTagName(bodyName);
  QDomNode bodyNode = bodyList.at(0);
  QDomElement bodyElement = bodyNode.toElement();

  if (!bodyElement.isNull()) {
    int rootId = 0;
    QString rootNodeName = KSILIT_TASKER_DOM_ELEMENT_TAG;
    rootNodeName += QString::number(rootId);
    QDomNodeList rootNodeList = domDoc.elementsByTagName(rootNodeName);
    QDomNode rootNode = rootNodeList.at(0);
    QDomElement rootElement = rootNode.toElement();
    QDomNode childNode = rootElement.firstChild();

    while (!childNode.isNull()) {
      QDomElement childElement = childNode.toElement();
      QString childTag = childElement.tagName();
      DBG3(PRINT_DBG("childTag: %s", qPrintable(childTag)));

      QString attributeChildId = childElement.attribute(KSILIT_TASKER_DOM_ELEMENT_ATTRIBUTE_ID);
      DBG3(PRINT_DBG("attributeChildId: %s", qPrintable(attributeChildId)));

      QString attributeChildName = childElement.attribute(KSILIT_TASKER_DOM_ELEMENT_ATTRIBUTE_NAME);
      DBG3(PRINT_DBG("attributeChildName: %s", qPrintable(attributeChildName)));

      QString attributeChildText = childElement.attribute(KSILIT_TASKER_DOM_ELEMENT_ATTRIBUTE_TEXT);
      DBG3(PRINT_DBG("attributeChildText: %s", qPrintable(attributeChildText)));

      QDomNode parentNode = childElement.parentNode();

      if (!parentNode.isNull()) {
        QDomElement parentElement = parentNode.toElement();

        if (!parentElement.isNull()) {
          int parentId = 0;
          QString attributeParentId = parentElement.attribute(KSILIT_TASKER_DOM_ELEMENT_ATTRIBUTE_ID);

          if (!attributeParentId.isEmpty()) {
            parentId = attributeParentId.toInt();
          }

          DBG3(PRINT_DBG("parentId: %i", parentId));

          QStandardItem *parentItem = rootTaskerItem;

          if (parentId) {
            parentItem = getItemById(rootTaskerItem, parentId);
          }

          if (parentItem) {
            QStandardItem *childItem = new QStandardItem();
            parentItem->appendRow(childItem);

            QVariant childData = attributeChildId;
            childItem->setData(childData);

            childItem->setText(attributeChildName);

            int childId = attributeChildId.toInt();
            tasker->createTask(childId);
            tasker->setName(childId, attributeChildName);
            tasker->setText(childId, attributeChildText);
          }
          else {
            DBGE(PRINT_ERROR("parentItem not found!"));
          }
        }
        else {
          DBGE(PRINT_ERROR("Error converting parent node to element!"));
        }
      }
      else {
        DBGE(PRINT_ERROR("parentNode is null!"));
      }

      childNode = getNextNode(childNode, rootNode);

      if (childNode.isNull()) {
        rv = ALL_OK;
      }
    }
  }
  else {
    DBGE(PRINT_ERROR("rootElement is null!"));
  }

  DBGR(PRINT_RETURN(""));
  return rv;
}

int DataBase::read(QString filePath) {
  DBGS(PRINT_START("filePath: %s", qPrintable(filePath)));

  int rv = ERROR_UNKNOWN_ERROR;

  if (isDBInitialised()) {
    QFile fileIn(filePath);

    if (fileIn.open(QIODevice::ReadOnly)) {
      if (domDoc.setContent(&fileIn)) {
        rv = readJotterHeader();

        if (rv == ALL_OK) {
          rv = readJotterBody();

          if (rv == ALL_OK) {
            rv = readTaskerHeader();

            if (rv == ALL_OK) {
              rv = readTaskerBody();
            }
            else {
              DBGE(PRINT_ERROR("Error reading tasker header!"));
            }
          }
          else {
            DBGE(PRINT_ERROR("Error reading jotter body!"));
          }
        }
        else {
          DBGE(PRINT_ERROR("Error reading jotter header!"));
        }
      }
      else {
        DBGE(PRINT_ERROR("Error setting DOM document content!"));
      }
    }
    else {
      DBGE(PRINT_ERROR("Error opening file: %s", qPrintable(filePath)));
      rv = ERROR_OPENING_FILE;
    }

    fileIn.close();
  }
  else {
    rv = ERROR_DB_NOT_INITIALIZED;
    DBGE(PRINT_ERROR("Data base not initialized!"));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}
