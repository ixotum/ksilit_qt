/*******************************************************************************
  FILE: ksilitunittest.h
  AUTOR: ixotum
  DATE: 2012-08-19 15:18:54
  DESCRIPTION: Definition of KsilitUnitTest class corresponded for unit testing
*******************************************************************************/

#ifndef KSILITUNITTEST_H
#define KSILITUNITTEST_H

#include <QObject>

class KsilitUnitTest : public QObject
{
  Q_OBJECT

public:
  KsilitUnitTest();

private slots:
  void jotParent();
  void jotInsertChildren();
  void jotColumnCount();
};

#endif // KSILITUNITTEST_H
