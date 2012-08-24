/*******************************************************************************
  FILE: mainwindow.h
  AUTOR: ixotum
  DATE: 2012-08-18 14:27:08
  DESCRIPTION: This file contains definition of MainWindow class
*******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void ksilitSlotHelpAbout();

private:
  void createConnections();

  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
