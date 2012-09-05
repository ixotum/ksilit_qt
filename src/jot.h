/*******************************************************************************
  FILE: jot.h
  AUTOR: ixotum
  DATE: 2012-09-04 20:26:33
  DESCRIPTION: This file contains definition of Jot class
*******************************************************************************/

#ifndef JOT_H
#define JOT_H

#include <QList>
#include <QVector>
#include <QVariant>

class Jot
{
public:
  Jot(Jot *parent = 0);
  Jot *parent() const;
  int childCount() const;
  int columnCount() const;

private:
  Jot *parentJot;
  QList<Jot *> children;
  QVector<QVariant> columnData;
};

#endif // JOT_H
