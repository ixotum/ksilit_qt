#include "task.h"

Task::Task()
{
}

QString Task::getName() {
  return name;
}

void Task::setName(QString taskName) {
  name = taskName;
}
