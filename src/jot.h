#ifndef JOT_H
#define JOT_H

#include <QString>

class Jot
{
public:
  Jot();
  QString getName();
  void setName(QString jotName);
  QString getText();
  void setText(QString jotText);

private:
  QString name;
  QString text;
};

#endif // JOT_H
