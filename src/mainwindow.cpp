#define MAINWINDOW_DBG_LVL 3
#define MAINWINDOW_DBG_OPTIONS DBG_W+DBG_E+DBG_S+DBG_R

#define MODULE_DBG_LVL MAINWINDOW_DBG_LVL
#define MODULE_DBG_OPTIONS MAINWINDOW_DBG_OPTIONS
#include "dbg.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QCloseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  dataBase(NULL)
{
  DBGS(PRINT_START(""));

  ui->setupUi(this);

  jotter = new Jotter();
  tasker = new Tasker();

  allocateActions();
  initActions();
  setupModel();
  createConnections();
  dataBaseInit();

  setKsilitSaveEnabled(false);
  setJotterRenameEnabled(false);
  setJotterTextChanged(false);
  ui->jotterNoteDescription->setEnabled(false);

  initSettings();
  initMainWindow();

  updateActions();

  DBGR(PRINT_RETURN(""));
}

MainWindow::~MainWindow()
{
  DBGS(PRINT_START(""));

  delete ui;

  DBGR(PRINT_RETURN(""));
}

void MainWindow::allocateActions() {
  DBGS(PRINT_START(""));

  ksilitActionFileNew    = new QAction(KSILIT_MAIN_WINDOW_MENU_FILE_NEW, this);
  ksilitActionFileOpen   = new QAction(KSILIT_MAIN_WINDOW_MENU_FILE_OPEN, this);
  ksilitActionFileSave   = new QAction(KSILIT_MAIN_WINDOW_MENU_FILE_SAVE, this);
  ksilitActionFileSaveAs = new QAction(KSILIT_MAIN_WINDOW_MENU_FILE_SAVE_AS, this);
  ksilitActionFileQuit   = new QAction(KSILIT_MAIN_WINDOW_MENU_FILE_QUIT, this);
  ksilitActionHelpAbout  = new QAction(KSILIT_MAIN_WINDOW_MENU_HELP_ABOUT, this);

  jotterActionAddNote    = new QAction(KSILIT_JOTTER_ACTION_ADD_NOTE, this);
  jotterActionAddSubNote = new QAction(KSILIT_JOTTER_ACTION_ADD_SUB_NOTE_TEXT, this);
  jotterActionMoveUp     = new QAction(KSILIT_JOTTER_ACTION_MOVE_UP, this);
  jotterActionMoveDown   = new QAction(KSILIT_JOTTER_ACTION_MOVE_DOWN, this);
  jotterActionDeleteNote = new QAction(KSILIT_JOTTER_ACTION_DELETE_NOTE_TEXT, this);

  taskerActionAddSubTask = new QAction(KSILIT_TASKER_ACTION_ADD_SUB_TASK, this);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::initActions() {
  DBGS(PRINT_START(""));

  ksilitActionFileOpen->setShortcut(Qt::CTRL + Qt::Key_O);
  ksilitActionFileSave->setShortcut(Qt::CTRL + Qt::Key_S);
  ksilitActionFileQuit->setShortcut(Qt::CTRL + Qt::Key_Q);

  jotterActionAddNote->setShortcut(Qt::CTRL + Qt::Key_N);
  jotterActionAddSubNote->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
  jotterActionMoveUp->setShortcut(Qt::CTRL + Qt::Key_Up);
  jotterActionMoveDown->setShortcut(Qt::CTRL + Qt::Key_Down);
  jotterActionDeleteNote->setShortcut(Qt::CTRL + Qt::Key_D);

  taskerActionAddSubTask->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::createConnections() {
  DBGS(PRINT_START(""));

  connect(ui->jotterTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(jotterContextMenuRequest(QPoint)));
  connect(ui->jotterTreeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(jotterSlotRowChanged(QModelIndex,QModelIndex)));

  QAbstractItemModel *model = ui->jotterTreeView->model();
  connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(jotterSlotItemRenamed()));

  connect(ksilitActionFileNew, SIGNAL(triggered()), this, SLOT(ksilitSlotFileNew()));
  connect(ksilitActionFileOpen, SIGNAL(triggered()), this, SLOT(ksilitSlotFileOpen()));
  connect(ksilitActionFileSave, SIGNAL(triggered()), this, SLOT(ksilitSlotFileSave()));
  connect(ksilitActionFileSaveAs, SIGNAL(triggered()), this, SLOT(ksilitSlotFileSaveAs()));
  connect(ksilitActionFileQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ksilitActionHelpAbout, SIGNAL(triggered()), this, SLOT(ksilitSlotHelpAbout()));

  connect(jotterActionAddNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddNote()));
  connect(jotterActionAddSubNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddSubNote()));
  connect(jotterActionMoveUp, SIGNAL(triggered()), this, SLOT(jotterSlotMoveUp()));
  connect(jotterActionMoveDown, SIGNAL(triggered()), this, SLOT(jotterSlotMoveDown()));
  connect(jotterActionDeleteNote, SIGNAL(triggered()), this, SLOT(jotterSlotDeleteNote()));

  connect(ui->jotterNoteDescription, SIGNAL(textChanged()), this, SLOT(jotterSlotTextChanged()));
  connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(ksilitSlotTabChanged(int)));

  connect(ui->taskerTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(taskerContextMenuRequest(QPoint)));
  connect(taskerActionAddSubTask, SIGNAL(triggered()), this, SLOT(taskerSlotAddSubTask()));

  DBGR(PRINT_RETURN(""));
}

void MainWindow::setupModel() {
  DBGS(PRINT_START(""));

  QStandardItemModel *jotterModel = new QStandardItemModel();
  ui->jotterTreeView->setModel(jotterModel);

  QStandardItem *jotterHeaderItem = new QStandardItem();
  jotterHeaderItem->setText(KSILIT_JOTTER_HEADER_COLUMN_NAME_TEXT);
  jotterModel->setHorizontalHeaderItem(KSILIT_JOTTER_HEADER_COLUMN_NAME_NUMBER, jotterHeaderItem);

  QStandardItemModel *taskerModel = new QStandardItemModel();
  ui->taskerTreeView->setModel(taskerModel);

  QStandardItem *taskerHeaderItem = new QStandardItem();
  taskerHeaderItem->setText(KSILIT_TASKER_HEADER_COLUMN_NAME_TEXT);
  taskerModel->setHorizontalHeaderItem(KSILIT_TASKER_HEADER_COLUMN_NAME_NUMBER, taskerHeaderItem);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::dataBaseInit() {
  DBGS(PRINT_START(""));

  QStandardItemModel *jotterModel = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QStandardItem *rootJotterItem = jotterModel->invisibleRootItem();
  //dataBase.setRootJotterItem(rootJotterItem);

  QStandardItemModel *taskerModel = static_cast<QStandardItemModel *>(ui->taskerTreeView->model());
  QStandardItem *rootTaskerItem = taskerModel->invisibleRootItem();
  //dataBase.setRootTaskerItem(rootTaskerItem);

  //dataBase.setJotter(jotter);
  //dataBase.setTasker(tasker);

  dataBase = new DataBase(jotter, rootJotterItem, tasker, rootTaskerItem);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::ksilitSlotHelpAbout() {
  DBGS(PRINT_START(""));

  QMessageBox mBox;
  QString text = "ksilit ";
  text += KSILIT_VERSION;
  mBox.setText(text);
  mBox.exec();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterContextMenuRequest(const QPoint &position) {
  DBGS(PRINT_START("position.x: %i, position.y: %i", position.x(), position.y()));

  QMenu jotterContextMenu(this);

  jotterContextMenu.addAction(jotterActionAddNote);
  jotterContextMenu.addAction(jotterActionAddSubNote);
  jotterContextMenu.addAction(jotterActionMoveUp);
  jotterContextMenu.addAction(jotterActionMoveDown);
  jotterContextMenu.addAction(jotterActionDeleteNote);

  updateActions();

  jotterContextMenu.exec(ui->jotterTreeView->viewport()->mapToGlobal(position));

  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterSlotAddNote() {
  DBGS(PRINT_START(""));

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();
  QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);
  QStandardItem *parentItem = selectedItem->parent();

  if (!parentItem) {
    parentItem = model->invisibleRootItem();
  }

  QStandardItem *childItem = new QStandardItem();
  parentItem->appendRow(childItem);

  QString childText = KSILIT_JOTTER_NEW_NOTE_TEXT;
  int childRow = childItem->row();
  childText += " ";
  childText += QString::number(childRow);
  childItem->setText(childText);

  int id = jotter->createJot();
  QVariant itemData = id;
  childItem->setData(itemData);

  jotter->setName(id, childText);

  setKsilitSaveEnabled(true);
  updateActions();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterSlotAddSubNote() {
  DBGS(PRINT_START(""));

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QModelIndex currentIndex = ui->jotterTreeView->selectionModel()->currentIndex();
  QStandardItem *parentItem = 0;

  if (currentIndex.isValid()) {
    parentItem = model->itemFromIndex(currentIndex);
  }
  else {
    parentItem = model->invisibleRootItem();
  }

  QStandardItem *childItem = new QStandardItem();
  parentItem->appendRow(childItem);

  if (parentItem == model->invisibleRootItem()) {
    QModelIndex childIndex = model->indexFromItem(childItem);
    ui->jotterTreeView->selectionModel()->setCurrentIndex(childIndex, QItemSelectionModel::ClearAndSelect);
  }

  QString childText = KSILIT_JOTTER_NEW_NOTE_TEXT;
  int childRow = childItem->row();
  childText += " ";
  childText += QString::number(childRow);
  setJotterRenameEnabled(false);
  childItem->setText(childText);

  int id = jotter->createJot();
  QVariant itemData = id;

  childItem->setData(itemData);
  setJotterRenameEnabled(true);

  jotter->setName(id, childText);

  setKsilitSaveEnabled(true);
  updateActions();

  DBGR(PRINT_RETURN(""));
}

int MainWindow::dataBaseSaveDialog() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  QFileDialog fileDialog;
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setNameFilter(KSILIT_DEFAULT_DATA_BASE_FILTER);
  fileDialog.setDefaultSuffix(KSILIT_DEFAULT_DATA_BASE_SUFFIX);
  fileDialog.selectFile(KSILIT_DEFAULT_DATA_BASE_NAME);

  if (fileDialog.exec()){
    QStringList fileList = fileDialog.selectedFiles();

    if (!fileList.isEmpty()) {
      dataBasePath = fileList.at(0);

      rv = dataBase->write(dataBasePath);
      if (rv) {
        DBGE(PRINT_ERROR("Error writing ksilit data base!"));
      }
    }
    else {
      DBGE(PRINT_ERROR("fileList is empty!"));
    }
  }
  else {
    //User just don't want to save his data
    rv = ERROR_ACTION_CANCELED;
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

void MainWindow::ksilitSlotFileSaveAs() {
  DBGS(PRINT_START(""));

  //Updating jotter text for current index
  int rv = jotterSaveCurrentText();

  if (rv == ALL_OK) {
    rv = dataBaseSaveDialog();

    if (rv) {
      DBGE(PRINT_ERROR("Error saving data base"));
    }
  }
  else {
    DBGE(PRINT_ERROR("Error saving current text!"));
  }

  if (rv == ALL_OK) {
    setSetting(KSILIT_SETTING_CATEGORY_GENERAL, KSILIT_SETTING_DEFAULT_PATH, dataBasePath);

    setKsilitSaveEnabled(false);
    updateActions();
  }

  DBGR(PRINT_RETURN(""))
}

void MainWindow::ksilitSlotFileOpen() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  QFileDialog fileDialog;
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog.setNameFilter(KSILIT_DEFAULT_DATA_BASE_FILTER);

  if (fileDialog.exec()) {
    QStringList fileList = fileDialog.selectedFiles();

    if (!fileList.isEmpty()) {
      dataBasePath = fileList.at(0);
      rv = ALL_OK;
    }
    else {
      DBGE(PRINT_ERROR("fileList is empty"));
    }
  }

  if (rv == ALL_OK) {
    ksilitReset();
    rv = ksilitLoadDataBase();

    if (rv == ALL_OK) {
      setSetting(KSILIT_SETTING_CATEGORY_GENERAL, KSILIT_SETTING_DEFAULT_PATH, dataBasePath);
    }
    else {
      DBGE(PRINT_ERROR("Error loading data base!"));
    }
  }

  updateMainWindowTitle();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::setJotterRenameEnabled(bool flag) {
  flagJotterRenameEnabled = flag;
}

bool MainWindow::isJotterRenameEnabled() {
  return flagJotterRenameEnabled;
}

void MainWindow::jotterSlotItemRenamed() {
  DBGS(PRINT_START(""));

  if (isJotterRenameEnabled()) {
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
    QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();
    QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);

    QString newName = selectedItem->text();
    DBG1(PRINT_DBG("newName: %s", qPrintable(newName)));

    int selectedId = selectedItem->data().toInt();
    jotter->setName(selectedId, newName);

    setKsilitSaveEnabled(true);
    updateActions();
  }

  DBGR(PRINT_RETURN(""));
}

void MainWindow::updateActions() {
  DBGS(PRINT_START(""));

  bool hasSelection = !ui->jotterTreeView->selectionModel()->selection().isEmpty();

  if (hasSelection) {
    jotterActionAddNote->setEnabled(true);
    jotterActionDeleteNote->setEnabled(true);
    jotterActionMoveUp->setEnabled(true);
    jotterActionMoveDown->setEnabled(true);
  }
  else {
    jotterActionAddNote->setEnabled(false);
    jotterActionDeleteNote->setEnabled(false);
    jotterActionMoveUp->setEnabled(false);
    jotterActionMoveDown->setEnabled(false);
  }

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();

  if (selectedIndex.isValid()) {
    QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);
    int selectedRow = selectedItem->row();

    if (selectedRow == 0) {
      jotterActionMoveUp->setEnabled(false);
    }

    QStandardItem *parentItem = selectedItem->parent();

    if (!parentItem) {
      parentItem = model->invisibleRootItem();
    }

    int childCount = parentItem->rowCount();
    int lastRow = childCount - 1;

    if (selectedRow == lastRow) {
      jotterActionMoveDown->setEnabled(false);
    }
  }

  if (isKsilitSaveEnabled()) {
    setWindowModified(true);

    ksilitActionFileSave->setEnabled(true);
  }
  else {
    setWindowModified(false);

    ksilitActionFileSave->setEnabled(false);
  }

  int rootChildCount = model->invisibleRootItem()->rowCount();
  if (rootChildCount) {
    ui->jotterNoteDescription->setEnabled(true);
  }
  else {
    ui->jotterNoteDescription->setEnabled(false);
  }

  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterSlotDeleteNote() {
  DBGS(PRINT_START(""));

  QMessageBox::StandardButton button = jotterDeleteRequest();

  if (button == QMessageBox::Ok) {
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
    QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();
    QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);
    QStandardItem *parentItem = selectedItem->parent();

    if (!parentItem) {
      DBG3(PRINT_DBG("parentItem is null"));
      parentItem = model->invisibleRootItem();
    }

    int selectedRow = selectedItem->row();
    parentItem->removeRow(selectedRow);

    setKsilitSaveEnabled(true);
    updateActions();
  }

  DBGR(PRINT_RETURN(""));
}

void MainWindow::ksilitSlotMenuEditUpdate() {
  DBGS(PRINT_START(""));
  updateActions();
  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterSlotMoveUp() {
  DBGS(PRINT_START(""));

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();
  QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);
  QStandardItem *parentItem = selectedItem->parent();

  if (!parentItem) {
    parentItem = model->invisibleRootItem();
  }

  int selectedRow = selectedItem->row();
  int previousRow = selectedRow - 1;
  parentItem->takeRow(selectedRow);
  parentItem->insertRow(previousRow, selectedItem);

  selectedIndex = model->indexFromItem(selectedItem);
  ui->jotterTreeView->selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::ClearAndSelect);

  setKsilitSaveEnabled(true);
  updateActions();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterSlotRowChanged(const QModelIndex &currentIndex, const QModelIndex &previousIndex) {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;
  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());

  //Saving previous index text
  if (previousIndex.isValid() && currentIndex.isValid()) {
    QStandardItem *previousItem = model->itemFromIndex(previousIndex);
    QVariant previousData = previousItem->data();
    int previousId = previousData.toInt();
    QTextDocument *previousDocument = ui->jotterNoteDescription->document();
    QString previousText = previousDocument->toPlainText();
    rv = jotter->setText(previousId, previousText);

    if (rv) {
      DBGE(PRINT_ERROR("Error saving previous text!"));
    }

    //Getting current index text
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    QVariant currentData = currentItem->data();
    int currentId = currentData.toInt();
    QString currentText = jotter->getText(currentId);

    setJotterTextChangingEnabled(false);
    ui->jotterNoteDescription->setPlainText(currentText);
    setJotterTextChangingEnabled(true);
  }

  updateActions();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterSlotMoveDown() {
  DBGS(PRINT_START(""));

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();
  QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);
  QStandardItem *parentItem = selectedItem->parent();

  if (!parentItem) {
    parentItem = model->invisibleRootItem();
  }

  int selectedRow = selectedItem->row();
  int nextRow = selectedRow + 1;
  parentItem->takeRow(selectedRow);
  parentItem->insertRow(nextRow, selectedItem);

  selectedIndex = model->indexFromItem(selectedItem);
  ui->jotterTreeView->selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::ClearAndSelect);

  setKsilitSaveEnabled(true);
  updateActions();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::ksilitReset() {
  DBGS(PRINT_START(""));

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QStandardItem *rootItem = model->invisibleRootItem();
  int childCount = rootItem->rowCount();

  rootItem->removeRows(0, childCount);

  setJotterTextChangingEnabled(false);
  ui->jotterNoteDescription->setPlainText(0);
  setJotterTextChangingEnabled(true);

  jotter->reset();

  DBGR(PRINT_RETURN(""));
}

bool MainWindow::isKsilitSaveEnabled() {
  return flagKsilitSaveEnabled;
}

void MainWindow::setKsilitSaveEnabled(bool flag) {
  flagKsilitSaveEnabled = flag;
}

void MainWindow::ksilitSlotFileSave() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  //Updating jotter text for current index
  rv = jotterSaveCurrentText();

  if (rv == ALL_OK) {
    if (dataBasePath.isEmpty()) {
      rv = dataBaseSaveDialog();
    }
    else {
      rv = dataBase->write(dataBasePath);
    }

    if (rv == ALL_OK) {
      setSetting(KSILIT_SETTING_CATEGORY_GENERAL, KSILIT_SETTING_DEFAULT_PATH, dataBasePath);

      setKsilitSaveEnabled(false);
      updateActions();
    }
    else if (rv == ERROR_ACTION_CANCELED) {
      DBGW(PRINT_WARNING("Saving canceled"));
    }
    else {
      DBGE(PRINT_ERROR("Error saving data base!"));
    }
  }
  else {
    DBGE(PRINT_ERROR("Error saving current text!"));
  }

  DBGR(PRINT_RETURN(""));
}

QMessageBox::StandardButton MainWindow::saveRequest() {
  DBGS(PRINT_START(""));

  QMessageBox::StandardButton button;
  button = QMessageBox::warning(this,
                                "ksilit",
                                KSILIT_WARNING_QUIT_SAVE,
                                QMessageBox::Discard | QMessageBox::Cancel | QMessageBox::Save);

  DBGR(PRINT_RETURN(""));
  return button;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  DBGS(PRINT_START(""));

  if (isKsilitSaveEnabled()) {
    QMessageBox::StandardButton button = saveRequest();

    if (button == QMessageBox::Save) {
      ksilitSlotFileSave();
    }
    else if (button == QMessageBox::Cancel) {
      event->ignore();
    }
    else if (button == QMessageBox::Discard) {
      event->accept();
    }
  }

  DBGR(PRINT_RETURN(""));
}

void MainWindow::jotterSlotTextChanged() {
  DBGS(PRINT_START(""));

  if (isJotterTextChangingEnabled()) {
    setJotterTextChanged(true);
    setKsilitSaveEnabled(true);
    updateActions();
  }

  DBGR(PRINT_RETURN(""));
}

void MainWindow::setJotterTextChanged(bool flag) {
  flagJotterTextChanged = flag;
}

//Saving current text to the according jot
//It's needed if we open some db, change current text and save it immediately
//Returns error if current index is invalid
int MainWindow::jotterSaveCurrentText() {
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;
  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();

  if (selectedIndex.isValid()) {
    QTextDocument *document = ui->jotterNoteDescription->document();
    QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);
    QVariant selectedData = selectedItem->data();
    int selectedId = selectedData.toInt();
    QString selectedText = document->toPlainText();
    jotter->setText(selectedId, selectedText);
    rv = ALL_OK;
  }
  else {
    DBGE(PRINT_ERROR("Current index is invalid!"));
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

bool MainWindow::isJotterTextChangingEnabled() {
  return flagJotterTextChangingEnabled;
}

void MainWindow::setJotterTextChangingEnabled(bool flag) {
  flagJotterTextChangingEnabled = flag;
}

void MainWindow::initMainWindowMenuFile() {
  DBGS(PRINT_START(""));

  QMenu *menuFile = new QMenu(KSILIT_MAIN_WINDOW_MENU_FILE);
  menuBar()->addMenu(menuFile);
  menuFile->addAction(ksilitActionFileNew);
  menuFile->addAction(ksilitActionFileOpen);
  menuFile->addAction(ksilitActionFileSave);
  menuFile->addAction(ksilitActionFileSaveAs);
  menuFile->addAction(ksilitActionFileQuit);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::updateMainWindowMenuEdit() {
  DBGS(PRINT_START(""));

  QMenu *menuEdit = new QMenu(KSILIT_MAIN_WINDOW_MENU_EDIT);
  menuBar()->addMenu(menuEdit);

  int currentTab = ui->tabWidget->currentIndex();

  switch (currentTab) {
  case KSILIT_JOTTER_TAB_INDEX :
    menuEdit->addAction(jotterActionAddNote);
    menuEdit->addAction(jotterActionAddSubNote);
    menuEdit->addAction(jotterActionMoveUp);
    menuEdit->addAction(jotterActionMoveDown);
    menuEdit->addAction(jotterActionDeleteNote);
    break;
  case KSILIT_TASKER_TAB_INDEX :
    menuEdit->addAction(taskerActionAddSubTask);
    break;
  }

  DBGR(PRINT_RETURN(""));
}

void MainWindow::initMainWindowMenuHelp() {
  DBGS(PRINT_START(""));

  QMenu *menuHelp = new QMenu(KSILIT_MAIN_WINDOW_MENU_HELP);
  menuBar()->addMenu(menuHelp);
  menuHelp->addAction(ksilitActionHelpAbout);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::updateMainWindowMenuBar() {
  DBGS(PRINT_START(""));

  menuBar()->clear();
  initMainWindowMenuFile();
  updateMainWindowMenuEdit();
  initMainWindowMenuHelp();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::updateMainWindowTitle() {
  DBGS(PRINT_START(""));

  QString dataBaseFile = dataBasePath.section('/', -1);//ksilit.ksi default
  QString mainWindowTitle = "[*]";
  mainWindowTitle += dataBaseFile;
  mainWindowTitle += " - ";
  mainWindowTitle += KSILIT_APPLICATION_NAME;
  mainWindowTitle += " ";
  mainWindowTitle += KSILIT_VERSION;
  setWindowTitle(mainWindowTitle);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::initMainWindow() {
  DBGS(PRINT_START(""));

  updateMainWindowTitle();
  updateMainWindowMenuBar();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::initSettings() {
  DBGS(PRINT_START(""));

  settings = new QSettings(KSILIT_ORGANIZATION_NAME, KSILIT_APPLICATION_NAME);

  QVariant value = getSetting(KSILIT_SETTING_CATEGORY_GENERAL, KSILIT_SETTING_DEFAULT_PATH);
  if (!value.isNull()) {
    dataBasePath = value.toString();
    ksilitLoadDataBase();
  }

  DBGR(PRINT_RETURN(""));
}

void MainWindow::setSetting(const QString &category, const QString &setting, const QVariant &value) {
  DBGS(PRINT_START("category: %s, setting: %s, value: %s", qPrintable(category), qPrintable(setting), qPrintable(value.toString())));

  QString key = category + "/" + setting;
  settings->setValue(key, value);

  DBGR(PRINT_RETURN(""));
}

QVariant MainWindow::getSetting(const QString &category, const QString &setting) {
  DBGS(PRINT_START("category: %s, setting: %s", qPrintable(category), qPrintable(setting)));

  QString key = category + "/" + setting;
  QVariant value = settings->value(key);

  DBGR(PRINT_RETURN("value: %s", qPrintable(value.toString())));
  return value;
}

int MainWindow::ksilitLoadDataBase(){
  DBGS(PRINT_START(""));

  int rv = ERROR_UNKNOWN_ERROR;

  setJotterRenameEnabled(false);
  rv = dataBase->read(dataBasePath);
  setJotterRenameEnabled(true);

  //Restore text for the first index
  if (rv == ALL_OK) {
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
    QStandardItem *rootItem = model->invisibleRootItem();

    if (rootItem->hasChildren()) {
      QStandardItem *firstChild = rootItem->child(0);
      QVariant firstData = firstChild->data();
      int firstId = firstData.toInt();
      QString firstText = jotter->getText(firstId);

      setJotterTextChangingEnabled(false);
      ui->jotterNoteDescription->setPlainText(firstText);
      setJotterTextChangingEnabled(true);

      QModelIndex firstIndex = model->indexFromItem(firstChild);
      ui->jotterTreeView->selectionModel()->setCurrentIndex(firstIndex, QItemSelectionModel::ClearAndSelect);
    }
  }

  DBGR(PRINT_RETURN("rv: %i", rv));
  return rv;
}

void MainWindow::ksilitSlotFileNew() {
  DBGS(PRINT_START(""));

  bool resetAll = false;

  if (isKsilitSaveEnabled()) {
    QMessageBox::StandardButton button = saveRequest();

    if (button == QMessageBox::Save) {
      ksilitSlotFileSave();
      resetAll = true;
    }
    else if (button == QMessageBox::Cancel) {
      resetAll = false;
    }
    else if (button == QMessageBox::Discard) {
      resetAll = true;
    }
  }
  else {
    resetAll = true;
  }

  if (resetAll) {
    ksilitReset();
    dataBasePath = "";
    setKsilitSaveEnabled(false);
    updateActions();
  }

  DBGR(PRINT_RETURN(""));
}

QMessageBox::StandardButton MainWindow::jotterDeleteRequest() {
    DBGS(PRINT_START(""));

    QMessageBox::StandardButton button;
    button = QMessageBox::warning(this,
                                  "ksilit",
                                  KSILIT_JOTTER_DELETE_REQUEST,
                                  QMessageBox::Ok | QMessageBox::Cancel);


    DBGR(PRINT_RETURN(""));
    return button;
}

void MainWindow::ksilitSlotTabChanged(const int &index) {
  DBGS(PRINT_START("index: %d", index));

  updateMainWindowMenuBar();

  DBGR(PRINT_RETURN(""));
}

void MainWindow::taskerContextMenuRequest(const QPoint &position) {
  DBGS(PRINT_START("position.x: %d, position.y: %d", position.x(), position.y()));

  QMenu taskerContextMenu(this);

  taskerContextMenu.addAction(taskerActionAddSubTask);

  updateActions();

  taskerContextMenu.exec(ui->taskerTreeView->viewport()->mapToGlobal(position));

  DBGR(PRINT_RETURN(""));
}

void MainWindow::setTaskerRenameEnabled(bool flag) {
  flagTaskerRenameEnabled = flag;
}

void MainWindow::taskerSlotAddSubTask() {
  DBGS(PRINT_START(""));

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->taskerTreeView->model());
  QModelIndex currentIndex = ui->taskerTreeView->selectionModel()->currentIndex();
  QStandardItem *parentItem = 0;

  if (currentIndex.isValid()) {
    parentItem = model->itemFromIndex(currentIndex);
  }
  else {
    parentItem = model->invisibleRootItem();
  }

  QStandardItem *childItem = new QStandardItem();
  parentItem->appendRow(childItem);

  if (parentItem == model->invisibleRootItem()) {
    QModelIndex childIndex = model->indexFromItem(childItem);
    ui->taskerTreeView->selectionModel()->setCurrentIndex(childIndex, QItemSelectionModel::ClearAndSelect);
  }

  QString childText = KSILIT_TASKER_NEW_TASK_TEXT;
  int childRow = childItem->row();
  childText += " ";
  childText += QString::number(childRow);
  setTaskerRenameEnabled(false);
  childItem->setText(childText);

  int id = tasker->createTask();
  QVariant itemData = id;

  childItem->setData(itemData);
  setTaskerRenameEnabled(true);

  tasker->setName(id, childText);

  setKsilitSaveEnabled(true);
  updateActions();

  DBGR(PRINT_RETURN(""));
}
