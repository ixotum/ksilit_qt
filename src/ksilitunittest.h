#ifndef KSILITUNITTEST_H
#define KSILITUNITTEST_H

#include <QObject>

class KsilitUnitTest : public QObject
{
  Q_OBJECT

public:
  KsilitUnitTest();

private slots:
  void jot_test_name_field();
  void jot_test_text();
  void jotter_test_jotCounter();
  void jotter_test_jot_creating();
  void jotter_test_text();
  void database_test_write();
  void database_test_read();
};

#endif // KSILITUNITTEST_H
