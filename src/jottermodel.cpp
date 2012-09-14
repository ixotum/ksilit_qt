/*******************************************************************************
  FILE: jottermodel.cpp
  AUTOR: ixotum
  DATE: 2012-09-04 20:28:18
  DESCRIPTION: This file contains implementation of JotterModel class
*******************************************************************************/
#define JOTTER_MODEL_DBG_LVL     3
#define JOTTER_MODEL_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL     JOTTER_MODEL_DBG_LVL
#define MODULE_DBG_OPTIONS JOTTER_MODEL_DBG_OPTIONS
#include "dbg.h"

#include "jottermodel.h"

JotterModel::JotterModel()
{
  rootJot = new Jot;
}

/*******************************************************************************
  NAME: index
  DESCRIPTION: overloaded function from QAbstractItemModel class. Creates an
    index by specified row, column and parent index
  ARGUMENTS:
  Input:
    int row - row inside parent index. Begins from 0 for every index.
    int column - column of new index
    const QModelIndex &parent - parent index
  Output:
    void
  RETURN VALUE:
    QModelIndex - returns new index if success, otherwise returns invalid index
*******************************************************************************/
QModelIndex JotterModel::index(int row, int column, const QModelIndex &parent) const {
  DBGS(PRINT_START("row: %i, column: %i, parent: 0x%08x", row, column, &parent));

  QModelIndex index;
  Jot *jotParent;

  jotParent = getJot(parent);
  if (jotParent) {
    Jot *jotChild = 0;
    jotChild = jotParent->child(row);
    if (jotChild) {
      index = createIndex(row, column, jotChild);
    }
    else {
      DBGE(PRINT_ERROR("jotChild with row: %i not found!", row));
    }
  }
  else {
    DBGE(PRINT_ERROR("jotParent with index: 0x%08x not found!", &index));
  }

  DBGR(PRINT_RETURN("index: 0x%08x", &index));
  return index;
}

/*******************************************************************************
  NAME: parent
  DESCRIPTION: Returns parentIndex from child. If child index is invalid or
    parent index equal to rootJot then returns empty QModelIndex
*******************************************************************************/
QModelIndex JotterModel::parent(const QModelIndex &child) const {
  DBGS(PRINT_START("child: 0x%08x", &child));

  QModelIndex parentIndex;
  Jot *childJot = 0;
  Jot *parentJot = 0;
  int childRow = 0;
  int childColumn = 0;

  if (child.isValid()) {
    childJot = getJot(child);

    if (childJot) {
      parentJot = childJot->parent();

      if (parentJot) {
        if (parentJot != rootJot) {
          childRow = child.row();
          childColumn = child.column();
          parentIndex = createIndex(childRow, childColumn, parentJot);
        }
        else {
          DBGW(PRINT_DBG("parentJot equal rootJot! Empty index will be return."));
        }
      }
      else {
        DBGE(PRINT_ERROR("parentJot is NULL!"));
      }
    }
    else {
      DBGE(PRINT_ERROR("childJot with index: 0x%08x not found!", &child));
    }
  }
  else {
    DBGW(PRINT_DBG("Child index is invalid! Emty index will be return."));
  }

  DBGR(PRINT_RETURN("parentIndex: 0x%08x", &parentIndex));
  return parentIndex;
}

/*******************************************************************************
  NAME: rowCount
  DESCRIPTION: Getting row count from the specified parent index
*******************************************************************************/
int JotterModel::rowCount(const QModelIndex &parent) const {
  DBGS(PRINT_START("parent: 0x%08x", &parent));

  int rowNumber = 0;
  Jot *parentJot = getJot(parent);

  if (parentJot) {
    rowNumber = parentJot->childCount();
  }
  else {
    DBGE(PRINT_ERROR("parentJot not found!"));
  }

  DBGR(PRINT_RETURN("rowNumber: %i", rowNumber));
  return rowNumber;
}

/*******************************************************************************
  NAME: columnCount
  DESCRIPTION: Getting column count from the specified parent index
*******************************************************************************/
int JotterModel::columnCount(const QModelIndex &parent) const {
  DBGS(PRINT_START("parent: 0x%08x", &parent));

  int columnNumber = 0;
  Jot *parentJot = getJot(parent);

  if (parentJot) {
    columnNumber = parentJot->columnCount();
  }
  else {
    DBGE(PRINT_ERROR("parentJot not found!"));
  }

  DBGR(PRINT_RETURN("columnNumber: %i", columnNumber));
  return columnNumber;
}

/*******************************************************************************
  NAME: data
  DESCRIPTION: get data from the index
  ARGUMENTS:
  Input:
    QModelIndex &index - reference to the index to getting data
    int role - must be Qt::DisplayRole
  Output:
    void
  RETURN VALUE:
    QVariant - got data
*******************************************************************************/
QVariant JotterModel::data(const QModelIndex &index, int role) const {
  DBGS(PRINT_START("index: 0x%08x, role: %i", &index, role));

  QVariant value;

  if (role == Qt::EditRole || role == Qt::DisplayRole) {
    Jot *jot = getJot(index);

    if (jot) {
      value = jot->getColumnData(index.column());
    }
    else {
      DBGE(PRINT_ERROR("jot with index: 0x%08x not found!", &index));
    }
  }
  else { //Often using another roles
    DBG3(PRINT_DBG("Used role: %i", role));
  }

  DBGR(PRINT_RETURN("value: 0x%08x", &value));
  return value;
}

/*******************************************************************************
  NAME: getJot
  DESCRIPTION: get jot by index
  ARGUMENTS:
  Input:
    QModelIndex &index - reference to the sought-for item
  Output:
    void
  RETURN VALUE:
    Jot * - found jot
*******************************************************************************/
Jot *JotterModel::getJot(const QModelIndex &index) const {
  DBGS(PRINT_START("index: 0x%08x", &index));

  Jot *jot = 0;

  if (index.isValid()) {
    jot = static_cast<Jot *>(index.internalPointer());
  }

  if (!jot) {
    jot = rootJot;
  }

  DBGR(PRINT_RETURN("jot: 0x%08x", jot));
  return jot;
}

/*******************************************************************************
  NAME: insertRows
  DESCRIPTION: insert rows to the parent
  ARGUMENTS:
  Input:
    int row - position to inserting
    int count - amount of rows
    const QModelIndex &parent - parent index
  Output:
    void
  RETURN VALUE:
    bool - true if success
*******************************************************************************/
bool JotterModel::insertRows(int row, int count, const QModelIndex &parent) {
  DBGS(PRINT_START("row: %i, count: %i, parent: 0x%08x", row, count, &parent));

  bool success = false;
  Jot *parentJot = 0;

  parentJot = getJot(parent);
  if (parentJot) {
    beginInsertRows(parent, row, row + count -1);
    success = parentJot->insertChildren(row, count);
    endInsertRows();

    if (!success) {
      DBGE(PRINT_ERROR("error inserting new children!"));
    }
  }
  else {
    DBGE(PRINT_ERROR("parentJot with index: 0x%08x not found!", &parent));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}

/*******************************************************************************
  NAME: insertColumns
  DESCRIPTION: insert columns to the rootJot recursively to the all children
  ARGUMENTS:
  Input:
    int column - position to inserting
    int count - amount of columns
    const QModelIndex &parent - parent index, but as fact will be use rootJot
  Output:
    void
  RETURN VALUE:
    bool - true if success
*******************************************************************************/
bool JotterModel::insertColumns(int column, int count, const QModelIndex &parent) {
  DBGS(PRINT_START("column: %i, count: %i, parent: 0x%08x", column, count, &parent));

  bool success = false;

  beginInsertColumns(parent, column, column + count - 1);
  success = rootJot->insertColumn(column, count);
  endInsertColumns();

  if (!success) {
    DBGE(PRINT_ERROR("error inserting new columns!"));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}

/*******************************************************************************
  NAME: setData
  DESCRIPTION: This is inherited function from the QAbstractItemModel class
  ARGUMENTS:
  Input:
    const QModelIndex &index - index where data will be setted
    const QVariant &value - value which contains data
    int role - must be Qt::EditRole
  Output:
    void
  RETURN VALUE:
    bool - true if success
*******************************************************************************/
bool JotterModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  DBGS(PRINT_START("index: 0x%08x, value: 0x%08x, role: %i", &index, &value, role));

  bool success = false;
  Jot *jot = getJot(index);

  if (role == Qt::EditRole) {
    if (jot) {
      success = jot->setColumnData(index.column(), value);

      if (success) {
        emit dataChanged(index, index);
      }
    }
    else {
      DBGE(PRINT_ERROR("jot with index: 0x%x not found!", &index));
    }
  }
  else {
    DBGE(PRINT_ERROR("invalid role!"));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}

/*******************************************************************************
  NAME: setHeaderData
  DESCRIPTION: This is inherited function from the QAbstractItemModel class
  ARGUMENTS:
  Input:
    int section - column in which header will be set
    Qt::Orientation orientation - horizontal orientation
    const QVariant &value - value for setting
    int role - must be Qt::EditRole
  Output:
    void
  RETURN VALUE:
    bool - true if success
*******************************************************************************/
bool JotterModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
  DBGS(PRINT_START("section: %i, orientation: %i, value: 0x%08x, role: %i", section, orientation, &value, role));

  bool success = false;

  if (orientation == Qt::Horizontal) {
    if (role == Qt::EditRole) {
      success = rootJot->setColumnData(section, value);

      if (success) {
        emit headerDataChanged(orientation, section, section);
      }
      else {
        DBGE(PRINT_ERROR("Error setting root column data!"));
      }
    }
    else {
      DBGE(PRINT_ERROR("role is invalid!"));
    }
  }
  else {
    DBGE(PRINT_ERROR("orientation is invalid!"));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}

/*******************************************************************************
  NAME: headerData
  DESCRIPTION: This is inherited function from the QAbstractItemModel class.
    Returns header data for the specified column
  ARGUMENTS:
  Input:
    int section - column in which header will be set
    Qt::Orientation orientation - horizontal orientation
    int role - must be Qt::EditRole
  Output:
    void
  RETURN VALUE:
    QVariant - returned data
*******************************************************************************/
QVariant JotterModel::headerData(int section, Qt::Orientation orientation, int role) const {
  DBGS(PRINT_START("section: %i, orientation: %i, role: %i", section, orientation, role));

  QVariant headerValue;

  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      headerValue = rootJot->getColumnData(section);
    }
  }
  else {
    DBGE(PRINT_ERROR("orientation is invalid!"));
  }

  DBGR(PRINT_RETURN("headerValue: 0x%08x", &headerValue));
  return headerValue;
}

/*******************************************************************************
  NAME: flags
  DESCRIPTION: This is inherited function from the QAbstractItemModel class.
    Returns the item flags for the given index.
  ARGUMENTS:
  Input:
    const QModelIndex &index - given index
  Output:
    void
  RETURN VALUE:
    Qt::ItemFlags - complex of flags
*******************************************************************************/
Qt::ItemFlags JotterModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = 0;

  if (index.isValid()) {
    flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  }

  return flags;
}
