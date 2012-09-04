/*******************************************************************************
  FILE: mainwindow.cpp
  AUTOR: ixotum
  DATE: 2012-08-18 14:27:37
  DESCRIPTION: This file contains implementation of MainWindow class
*******************************************************************************/
#define MAINWINDOW_DBG_LVL     0
#define MAINWINDOW_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL     MAINWINDOW_DBG_LVL
#define MODULE_DBG_OPTIONS MAINWINDOW_DBG_OPTIONS
#include "dbg.h"

#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  allocateActions();
  createConnections();
}

MainWindow::~MainWindow()
{
  delete ui;
}

/*******************************************************************************
  NAME: allocateActions
  DESCRIPTION: Allocating memory for all actions
  ARGUMENTS:
  Input:
    void
  Output:
    void
  RETURN VALUE:
    void
*******************************************************************************/
void MainWindow::allocateActions() {
  DBGS(PRINT_START());

  actionJotterAddSubNote = new QAction(KSILIT_JOTTER_ACTION_ADD_SUB_NOTE_TEXT, this);

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: createConnections
  DESCRIPTION: Creating connections to the slots
  ARGUMENTS:
  Input:
    void
  Output:
    void
  RETURN VALUE:
    void
*******************************************************************************/
void MainWindow::createConnections() {
  DBGS(PRINT_START("void"));

  connect(ui->action_About, SIGNAL(triggered()), this, SLOT(ksilitSlotHelpAbout()));
  connect(ui->jotterTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(jotterMenuRequest(QPoint)));

  connect(actionJotterAddSubNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddSubNote()));

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: jotterMenuRequest
  DESCRIPTION: Creating context menu for the jotter's tree view
  ARGUMENTS:
  Input:
    const QPoint &position - position of the mouse inside jotterTreeView
  Output:
    void
  RETURN VALUE:
    void
*******************************************************************************/
void MainWindow::jotterMenuRequest(const QPoint &position) {
  DBGS(PRINT_START("position x: %i, position y: %i", position.x(), position.y()));

  QMenu jotterContextMenu(this);

  jotterContextMenu.addAction(actionJotterAddSubNote);

  jotterContextMenu.exec(ui->jotterTreeView->viewport()->mapToGlobal(position));

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: ksilitSlotHelpAbout
  DESCRIPTION: Showing Help->About message
  ARGUMENTS:
  Input:
    void
  Output:
    void
  RETURN VALUE:
    void
*******************************************************************************/
void MainWindow::ksilitSlotHelpAbout() {
  DBGS(PRINT_START("void"));

  QMessageBox mBox;

  mBox.setText(QString("ksilit: ") + KSILIT_VERSION);
  mBox.exec();

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: jotterSlotAddSubNote
  DESCRIPTION: Implementation of adding child to the jotter's tree view slot
  ARGUMENTS:
  Input:
    void
  Output:
    void
  RETURN VALUE:
    void
*******************************************************************************/
void MainWindow::jotterSlotAddSubNote() {
  DBGS(PRINT_START());
  DBGR(PRINT_RETURN());
}
