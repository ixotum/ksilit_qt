/*******************************************************************************
  FILE: mainwindow.cpp
  AUTOR: ixotum
  DATE: 2012-08-18 14:27:37
  DESCRIPTION: This file contains implementation of MainWindow class
*******************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}
