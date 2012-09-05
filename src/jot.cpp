/*******************************************************************************
  FILE: jot.cpp
  AUTOR: ixotum
  DATE: 2012-09-04 20:27:47
  DESCRIPTION: This file contains implementation of Jot class
*******************************************************************************/
#define JOT_DBG_LVL     0
#define JOT_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL     JOT_DBG_LVL
#define MODULE_DBG_OPTIONS JOT_DBG_OPTIONS
#include "dbg.h"

#include "jot.h"

Jot::Jot(Jot *parent)
{
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
