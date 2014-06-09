#ifndef TASK_H
#define TASK_H

#include <QString>

class Task
{
public:
  Task();
  QString getName();
  void setName(QString taskName);
  QString getText();
  void setText(QString taskText);

private:
  QString name;
  QString text;
};

#endif // TASK_H
