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

QModelIndex JotterModel::parent(const QModelIndex &child) const {
  //dummy
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

int JotterModel::columnCount(const QModelIndex &parent) const {
  //dummy
}

QVariant JotterModel::data(const QModelIndex &index, int role) const {
  //dummy
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
      DBGE(PRINT_ERROR("can't insert new jot!"));
    }
  }
  else {
    DBGE(PRINT_ERROR("parentJot with index: 0x%08x not found!", &parent));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}
