#define TASKER_DBG_LVL 0
#define TASKER_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL TASKER_DBG_LVL
#define MODULE_DBG_OPTIONS TASKER_DBG_OPTIONS
#include "dbg.h"

#include "tasker.h"
#include "defines.h"

Tasker::Tasker()
{
  taskCounter = 0;
}

void Tasker::incTaskCounter() {
  ++taskCounter;
}

int Tasker::getTaskCount() {
  return taskCounter;
}

int Tasker::setTaskCount(int num) {
  int rv = ERROR_UNKNOWN_ERROR;
  if (num >= 0) {
    taskCounter = num;
    rv = ALL_OK;
  }

  return rv;
}

QString Tasker::getText(int id) {
  DBGS(PRINT_START("id: %i", id));

  QString text;
  Task *task = getTask(id);

  if (task) {
    text = task->getText();
  }
  else {
    DBGE(PRINT_ERROR("Task with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("text: %s", qPrintable(text)));
  return text;
}

int Tasker::setText(int id, QString taskText) {
  DBGS(PRINT_START("id: %i, taskText: %s", id, qPrintable(taskText)));

  int rv = ERROR_UNKNOWN_ERROR;
  Task *task = getTask(id);

  if (task) {
    task->setText(taskText);
    rv = ALL_OK;
  }
  else {
    DBGE(PRINT_ERROR("Task with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

//Return id of a new task
//if useId == -1, next id from taskCounter will be used
//else id = useId
int Tasker::createTask(int useId) {
  DBGS(PRINT_START(""));

  int id = 0;
  Task *task = new Task();

  if (useId == -1) {
    incTaskCounter();
    id = getTaskCount();
  }
  else {
    id = useId;
  }

  map.insert(id, task);

  DBGR(PRINT_RETURN("id: %i", id));
  return id;
}

//Returns NULL if task not found
Task *Tasker::getTask(int id) {
  DBGS(PRINT_START("id: %i", id));

  Task *task = NULL;

  if (map.contains(id)) {
    task = map.value(id);
  }
  else {
    DBGE(PRINT_ERROR("Task with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("task: %p", task));
  return task;
}

QString Tasker::getName(int id) {
  DBGS(PRINT_START("id: %i", id));

  QString name;

  Task *task = getTask(id);

  if (task) {
    name = task->getName();
  }
  else {
    DBGE(PRINT_ERROR("Task with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("Found name: %s", qPrintable(name)));
  return name;
}

int Tasker::setName(int id, QString taskName) {
  DBGS(PRINT_START("id: %i, taskName: %s", id, qPrintable(taskName)));

  int rv = ERROR_UNKNOWN_ERROR;
  Task *task = getTask(id);

  if (task) {
    task->setName(taskName);
    rv = ALL_OK;
  }
  else {
    DBGE(PRINT_ERROR("Task with id: %i not found!", id));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}
