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
#include <QFileDialog>
#include <QTextStream>

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
  actionJotterAddNote = new QAction(KSILIT_JOTTER_ACTION_ADD_NOTE_TEXT, this);

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: createConnections
  DESCRIPTION: Creating connections to the slots
*******************************************************************************/
void MainWindow::createConnections() {
  DBGS(PRINT_START("void"));

  connect(ui->action_About, SIGNAL(triggered()), this, SLOT(ksilitSlotHelpAbout()));
  connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(ksilitSlotFileSaveAs()));
  connect(ui->jotterTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(jotterMenuRequest(QPoint)));

  connect(actionJotterAddSubNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddSubNote()));
  connect(actionJotterAddNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddNote()));

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
  NAME: jotterUpdateActions
  DESCRIPTION: Updating the states of jotter's actions
*******************************************************************************/
void MainWindow::jotterUpdateActions() {
  DBGS(PRINT_START());

  bool hasSelection = !ui->jotterTreeView->selectionModel()->selection().isEmpty();

  actionJotterAddNote->setEnabled(hasSelection);

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: jotterMenuRequest
  DESCRIPTION: Creating context menu for the jotter's tree view
*******************************************************************************/
void MainWindow::jotterMenuRequest(const QPoint &position) {
  DBGS(PRINT_START("position x: %i, position y: %i", position.x(), position.y()));

  QMenu jotterContextMenu(this);

  jotterContextMenu.addAction(actionJotterAddNote);
  jotterContextMenu.addAction(actionJotterAddSubNote);

  jotterUpdateActions();
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

int MainWindow::ksilitDataBaseWrite(QString dataBasePath) {
  DBGS(PRINT_START("dataBasePath: %s", qPrintable(dataBasePath)));

  int rv = ERROR_UNKNOWN_ERROR;
  JotterModel *model = static_cast<JotterModel *>(ui->jotterTreeView->model());
  QDomDocument *domDoc = model->getDomDocument();
  QDomElement rootElement = domDoc->firstChildElement();

  if (!rootElement.isNull()) {
    QString xmlString = domDoc->toString();

    if (!xmlString.isNull()) {
      QFile fileOut(dataBasePath);

      if (fileOut.open(QIODevice::WriteOnly)) {
        QTextStream outStream(&fileOut);
        outStream << xmlString;
        fileOut.close();
        rv = ALL_OK;
      }
      else {
        DBGE(PRINT_ERROR("Error opening data base: %s!", qPrintable(dataBasePath)));
        rv = ERROR_OPENING_FILE;
      }
    }
    else {
      DBGE(PRINT_ERROR("xmlString is empty!"))
    }
  }
  else {
    DBGE(PRINT_ERROR("The root element of the model is empty!"));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

void MainWindow::ksilitSlotFileSaveAs() {
  DBGS(PRINT_START());

  int rv = ERROR_UNKNOWN_ERROR;

  QFileDialog fileDialog;
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setNameFilter(KSILIT_DEFAULT_DATA_BASE_FILTER);
  fileDialog.setDefaultSuffix(KSILIT_DEFAULT_DATA_BASE_SUFFIX);
  fileDialog.selectFile(KSILIT_DEFAULT_DATA_BASE_NAME);

  if (fileDialog.exec()) {
    QStringList fileList = fileDialog.selectedFiles();

    if (!fileList.isEmpty()) {
      QString dataBasePath = fileList.at(0);
      rv = ksilitDataBaseWrite(dataBasePath);

      if (rv) {
        DBGE(PRINT_ERROR("Error writing ksilit data base!"));
      }
    }
    else {
      DBGW(PRINT_DBG("fileList is empty!"));
    }
  }

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

  childPosition = model->rowCount(selectedIndex);
  model->insertRow(childPosition, selectedIndex);

  QModelIndex childIndex = model->index(childPosition, childColumn, selectedIndex);
  if (childIndex.isValid()) {
    QVariant subNoteText = KSILIT_JOTTER_SUB_NOTE_TEXT;
    QString postfix;
    subNoteText = subNoteText.toString() + " "+ postfix.setNum(childPosition);
    model->setData(childIndex, subNoteText, Qt::EditRole);
  }
  else {
    DBGE(PRINT_ERROR("childIndex is invalid!"));
  }

  DBGR(PRINT_RETURN());
}

/*******************************************************************************
  NAME: jotterSlotAddNote
  DESCRIPTION: Implementation of adding sibling to the current selected jotter's
    tree view note
*******************************************************************************/
void MainWindow::jotterSlotAddNote() {
  DBGS(PRINT_START());

  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();
  QAbstractItemModel *model = ui->jotterTreeView->model();
  QModelIndex parentIndex = selectedIndex.parent();
  int siblingPosition = 0;
  int columnPosition = 0;

  siblingPosition = model->rowCount(parentIndex);
  model->insertRow(siblingPosition, parentIndex);

  QModelIndex siblingIndex = model->index(siblingPosition, columnPosition, parentIndex);
  if (siblingIndex.isValid()) {
    QVariant noteText = KSILIT_JOTTER_SUB_NOTE_TEXT;
    QString postfix;
    noteText = noteText.toString() + " " + postfix.setNum(siblingPosition);
    model->setData(siblingIndex, noteText, Qt::EditRole);
  }
  else {
    DBGE(PRINT_ERROR("siblingIndex is inavalid!"));
  }

  DBGR(PRINT_RETURN());
}
