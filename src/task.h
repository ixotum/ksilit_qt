#ifndef TASK_H
#define TASK_H

#include <QString>

class Task
{
public:
  Task();
  QString getName();
  void setName(QString taskName);

private:
  QString name;
};

#endif // TASK_H
