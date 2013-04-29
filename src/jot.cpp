#define JOT_DBG_LVL 0
#define JOT_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL JOT_DBG_LVL
#define MODULE_DBG_OPTIONS JOT_DBG_OPTIONS
#include "dbg.h"

#include "jot.h"
#include "defines.h"

Jot::Jot()
{
  DBGS(PRINT_START(""));
  DBGR(PRINT_RETURN(""));
}

QString Jot::getName() {
  return name;
}

void Jot::setName(QString jotName) {
  name = jotName;
}

QString Jot::getText() {
  return text;
}

void Jot::setText(QString jotText) {
  text = jotText;
}
