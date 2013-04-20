#ifndef JOTTER_H
#define JOTTER_H

#include <QMap>

#include "jot.h"

class Jotter
{
public:
  Jotter();
  int getJotCount();
  int setJotCount(int num);
  int createJot(int useId = -1);
  QString getName(int id);
  int setName(int id, QString jotName);
  void reset();
  QString getText(int id);
  int setText(int id, QString jotText);

private:
  void incJotCounter();
  Jot *getJot(int id);

  int jotCounter;
  QMap <int, Jot *> map;
};

#endif // JOTTER_H
