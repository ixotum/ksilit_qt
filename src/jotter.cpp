#define JOTTER_DBG_LVL 0
#define JOTTER_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL JOTTER_DBG_LVL
#define MODULE_DBG_OPTIONS JOTTER_DBG_OPTIONS
#include "dbg.h"

#include <QString>

#include "jotter.h"
#include "defines.h"

Jotter::Jotter()
{
  jotCounter = 0;
}

void Jotter::incJotCounter() {
  ++jotCounter;
}

int Jotter::getJotCount() {
  return jotCounter;
}

int Jotter::setJotCount(int num) {
  int rv = ERROR_UNKNOWN_ERROR;
  if (num >= 0) {
    jotCounter = num;
    rv = ALL_OK;
  }

  return rv;
}

//Return id of a new jot
//if useId == -1, next id from jotCounter will be used
//else id = useId
int Jotter::createJot(int useId) {
  DBGS(PRINT_START(""));

  int id = 0;
  Jot *jot = new Jot();

  if (useId == -1) {
    incJotCounter();
    id = getJotCount();
  }
  else {
    id = useId;
  }

  map.insert(id, jot);

  DBGR(PRINT_RETURN("id: %i", id));
  return id;
}

//Returns 0 if jot not found
Jot *Jotter::getJot(int id) {
  DBGS(PRINT_START("id: %i", id));

  Jot *jot = 0;

  if (map.contains(id)) {
    jot = map.value(id);
  }
  else {
    DBGE(PRINT_ERROR("Jot with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("jot: 0x%08x", jot));
  return jot;
}

QString Jotter::getName(int id) {
  DBGS(PRINT_START("id: %i", id));

  QString name;

  Jot *jot = getJot(id);

  if (jot) {
    name = jot->getName();
  }
  else {
    DBGE(PRINT_ERROR("Jot with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("Found name: %s", qPrintable(name)));
  return name;
}

int Jotter::setName(int id, QString jotName) {
  DBGS(PRINT_START("id: %i, jotName: %s", id, qPrintable(jotName)));

  int rv = ERROR_UNKNOWN_ERROR;
  Jot *jot = getJot(id);

  if (jot) {
    jot->setName(jotName);
    rv = ALL_OK;
  }
  else {
    DBGE(PRINT_ERROR("Jot with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

void Jotter::reset() {
  DBGS(PRINT_START(""));

  map.clear();
  setJotCount(0);

  DBGR(PRINT_RETURN(""));
}

QString Jotter::getText(int id) {
  DBGS(PRINT_START("id: %i", id));

  QString text;
  Jot *jot = getJot(id);

  if (jot) {
    text = jot->getText();
  }
  else {
    DBGE(PRINT_ERROR("Jot with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("text: %s", qPrintable(text)));
  return text;
}

int Jotter::setText(int id, QString jotText) {
  DBGS(PRINT_START("id: %i, jotText: %s", id, qPrintable(jotText)));

  int rv = ERROR_UNKNOWN_ERROR;
  Jot *jot = getJot(id);

  if (jot) {
    jot->setText(jotText);
    rv = ALL_OK;
  }
  else {
    DBGE(PRINT_ERROR("Jot with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}
