/*******************************************************************************
  FILE: jot.cpp
  AUTOR: ixotum
  DATE: 2012-09-04 20:27:47
  DESCRIPTION: This file contains implementation of Jot class
*******************************************************************************/
#define JOT_DBG_LVL     3
#define JOT_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL     JOT_DBG_LVL
#define MODULE_DBG_OPTIONS JOT_DBG_OPTIONS
#include "dbg.h"

#include "jot.h"

Jot::Jot(Jot *parent)
{
  QVector<QVariant> data(1);
  columnData = data;
  parentJot = parent;
}

/*******************************************************************************
  NAME: parent
  DESCRIPTION: Returns parentJot
*******************************************************************************/
Jot *Jot::parent() const {
  DBGS(PRINT_START());
  DBGR(PRINT_RETURN("parentJot: 0x%08x", parentJot));
  return parentJot;
}

/*******************************************************************************
  NAME: childCount
  DESCRIPTION: Returns amount of children
*******************************************************************************/
int Jot::childCount() const {
  DBGS(PRINT_START());
  DBGR(PRINT_RETURN("child count: %i", children.count()));
  return children.count();
}

/*******************************************************************************
  NAME: columnCount
  DESCRIPTION: Returns amount of columns
*******************************************************************************/
int Jot::columnCount() const {
  DBGS(PRINT_START());
  DBGR(PRINT_RETURN("child column: %i", columnData.count()));
  return columnData.count();
}

/*******************************************************************************
  NAME: insertChildren
  DESCRIPTION: inserting new children to the current Jot
  ARGUMENTS:
  Input:
    int position - position for inserting
    int count - amount of children
  Output:
    void
  RETURN VALUE:
    bool - true if success
*******************************************************************************/
bool Jot::insertChildren(int position, int count) {
  DBGS(PRINT_START("position: %i, count: %i", position, count));

  bool success = false;
  int childrenCount = childCount();
  DBG3(PRINT_DBG("childrenCount: %i", childrenCount));

  if (position >= 0 && position <= childrenCount) {
    for (int i = 0; i < count; ++i) {
      Jot *newJot = new Jot(this);
      children.insert(position, newJot);
    }
    success = true;
  }
  else {
    DBGE(PRINT_ERROR("position is invalid!"));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}

/*******************************************************************************
  NAME: insertColumn
  DESCRIPTION: inserting new columns to the specified position
  ARGUMENTS:
  Input:
    int position - position for inserting
    int count - amount of columns
  Output:
    void
  RETURN VALUE:
    bool - true if success
*******************************************************************************/
bool Jot::insertColumn(int position, int count) {
  DBGS(PRINT_START("position: %i, count: %i", position, count));

  bool success = false;
  int columnNumber = columnCount();

  if (position >= 0 && position <= columnNumber) {
    for (int i = 0; i < count; ++i) {
      QVariant newColumn;
      columnData.insert(position, newColumn);
    }

    foreach (Jot *jotItem, children) {
      jotItem->insertColumn(position, count);
    }

    success = true;
  }
  else {
    DBGE(PRINT_ERROR("position is invalid!"));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}

/*******************************************************************************
  NAME: getColumnData
  DESCRIPTION: getting data from the specified column
  ARGUMENTS:
  Input:
    int column - column for getting data
  Output:
    void
  RETURN VALUE:
    QVariant - returned data
*******************************************************************************/
QVariant Jot::getColumnData(int column) const {
  DBGS(PRINT_START("column: %i", column));

  QVariant data = 0;
  int columnNumber = columnCount();

  if (column >= 0 && column <= columnNumber) {
    data = columnData.value(column);
  }
  else {
    DBGE(PRINT_ERROR("column number is invalid!"));
  }

  DBGR(PRINT_RETURN("data: %i", data.toInt()));
  return data;
}

/*******************************************************************************
  NAME: setColumnData
  DESCRIPTION: setting data from the value to the specified column
  ARGUMENTS:
  Input:
    int column - column for setting data
    QVariant value - value
  Output:
    void
  RETURN VALUE:
    bool - true if success
*******************************************************************************/
bool Jot::setColumnData(int column, QVariant value) {
  DBGS(PRINT_START("column: %i, value: %i", column, value.toInt()));

  bool success = false;
  int columnNumber = columnCount();

  if (column >= 0 && column <= columnNumber) {
    columnData[column] = value;
    success = true;
  }
  else {
    DBGE(PRINT_ERROR("column number is invalid!"));
  }

  DBGR(PRINT_RETURN("success: %s", success ? "true" : "false"));
  return success;
}

/*******************************************************************************
  NAME: child
  DESCRIPTION: Returns found child or 0
*******************************************************************************/
Jot *Jot::child(int number) const {
  DBGS(PRINT_START("number: %i", number));

  Jot *childJot = 0;

  if (number >= 0 && number <= childCount()) {
    childJot = children.value(number);
  }
  else {
    DBGE(PRINT_ERROR("number of child is invalid!"));
  }

  DBGR(PRINT_RETURN("childJot: 0x%08x", childJot));
  return childJot;
}