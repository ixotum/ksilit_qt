#ifndef TASKER_H
#define TASKER_H

#include <QMap>

#include "task.h"

class Tasker
{
public:
  Tasker();
  int createTask(int useId = -1);
  QString getName(int id);
  int setName(int id, QString taskName);
  int getTaskCount();
  int setTaskCount(int num);
  QString getText(int id);
  int setText(int id, QString taskText);

private:
  void incTaskCounter();
  Task *getTask(int id);

  int taskCounter;
  QMap <int, Task *> map;
};

#endif // TASKER_H
