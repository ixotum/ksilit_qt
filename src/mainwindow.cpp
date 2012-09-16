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
#include "jottermodel.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  allocateActions();
  createConnections();
  setupModel();
}

MainWindow::~MainWindow()
{
  delete ui;
}

/*******************************************************************************
  NAME: allocateActions
  DESCRIPTION: Allocating memory for all actions
*******************************************************************************/
void MainWindow::allocateActions() {
  DBGS(PRINT_START());

  actionJotterAddSubNote = new QAction(KSILIT_JOTTER_ACTION_ADD_SUB_NOTE_TEXT, this);

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: createConnections
  DESCRIPTION: Creating connections to the slots
*******************************************************************************/
void MainWindow::createConnections() {
  DBGS(PRINT_START("void"));

  connect(ui->action_About, SIGNAL(triggered()), this, SLOT(ksilitSlotHelpAbout()));
  connect(ui->jotterTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(jotterMenuRequest(QPoint)));

  connect(actionJotterAddSubNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddSubNote()));

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: setupModel
  DESCRIPTION: Setup model for jotter tree view
*******************************************************************************/
void MainWindow::setupModel() {
  DBGS(PRINT_START());

  JotterModel *model = new JotterModel;
  ui->jotterTreeView->setModel(model);
  QVariant value = KSILIT_JOTTER_COLUMN_NAME_TEXT;
  bool success = model->setHeaderData(KSILIT_JOTTER_COLUMN_NAME_NUMBER, Qt::Horizontal, value, Qt::EditRole);
  if (!success) {
    DBGE(PRINT_ERROR("Error setting jotter's tree view header!"));
  }

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: jotterMenuRequest
  DESCRIPTION: Creating context menu for the jotter's tree view
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
*******************************************************************************/
void MainWindow::jotterSlotAddSubNote() {
  DBGS(PRINT_START());

  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();
  QAbstractItemModel *model = ui->jotterTreeView->model();
  int childPosition = 0;
  int childColumn = 0;

  model->insertRow(childPosition, selectedIndex);
  QModelIndex childIndex = model->index(childPosition, childColumn, selectedIndex);
  if (childIndex.isValid()) {
    QVariant subNoteText = KSILIT_JOTTER_SUB_NOTE_TEXT;
    model->setData(childIndex, subNoteText, Qt::EditRole);
  }
  else {
    DBGE(PRINT_ERROR("childIndex is invalid!"));
  }

  DBGR(PRINT_RETURN());
}
