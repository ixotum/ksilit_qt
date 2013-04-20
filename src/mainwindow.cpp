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
  ui(new Ui::MainWindow)
{
  DBGS(PRINT_START(""));

  ui->setupUi(this);

  jotter = new Jotter();

  allocateActions();
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

  jotterActionAddNote = new QAction(KSILIT_JOTTER_ACTION_ADD_NOTE, this);
  jotterActionAddSubNote = new QAction(KSILIT_JOTTER_ACTION_ADD_SUB_NOTE_TEXT, this);
  jotterActionMoveUp = new QAction(KSILIT_JOTTER_ACTION_MOVE_UP, this);
  jotterActionMoveDown = new QAction(KSILIT_JOTTER_ACTION_MOVE_DOWN, this);
  jotterActionDeleteNote = new QAction(KSILIT_JOTTER_ACTION_DELETE_NOTE_TEXT, this);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::createConnections() {
  DBGS(PRINT_START(""));

  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(ksilitSlotHelpAbout()));
  connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(ksilitSlotFileSaveAs()));
  connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(ksilitSlotOpen()));
  connect(ui->actionAdd_Note, SIGNAL(triggered()), this, SLOT(jotterSlotAddNote()));
  connect(ui->actionAdd_Sub_Note, SIGNAL(triggered()), this, SLOT(jotterSlotAddSubNote()));
  connect(ui->action_Delete_Note, SIGNAL(triggered()), this, SLOT(jotterSlotDeleteNote()));
  connect(ui->actionMove_Up, SIGNAL(triggered()), this, SLOT(jotterSlotMoveUp()));
  connect(ui->actionMove_Down, SIGNAL(triggered()), this, SLOT(jotterSlotMoveDown()));
  connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(ksilitSlotFileSave()));
  connect(ui->menu_Edit, SIGNAL(aboutToShow()), this, SLOT(ksilitSlotMenuEditUpdate()));
  connect(ui->action_New, SIGNAL(triggered()), this, SLOT(ksilitSlotFileNew()));

  connect(ui->jotterTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(jotterContextMenuRequest(QPoint)));
  connect(ui->jotterTreeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(jotterSlotRowChanged(QModelIndex,QModelIndex)));

  QAbstractItemModel *model = ui->jotterTreeView->model();
  connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(jotterSlotItemRenamed()));

  connect(jotterActionAddNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddNote()));
  connect(jotterActionAddSubNote, SIGNAL(triggered()), this, SLOT(jotterSlotAddSubNote()));
  connect(jotterActionMoveUp, SIGNAL(triggered()), this, SLOT(jotterSlotMoveUp()));
  connect(jotterActionMoveDown, SIGNAL(triggered()), this, SLOT(jotterSlotMoveDown()));
  connect(jotterActionDeleteNote, SIGNAL(triggered()), this, SLOT(jotterSlotDeleteNote()));

  connect(ui->jotterNoteDescription, SIGNAL(textChanged()), this, SLOT(jotterSlotTextChanged()));

  DBGR(PRINT_RETURN(""));
}

void MainWindow::setupModel() {
  DBGS(PRINT_START(""));

  QStandardItemModel *jotterModel = new QStandardItemModel();
  ui->jotterTreeView->setModel(jotterModel);

  QStandardItem *headerItem = new QStandardItem;
  headerItem->setText(KSILIT_JOTTER_HEADER_COLUMN_NAME_TEXT);
  jotterModel->setHorizontalHeaderItem(KSILIT_JOTTER_HEADER_COLUMN_NAME_NUMBER, headerItem);

  DBGR(PRINT_RETURN(""));
}

void MainWindow::dataBaseInit() {
  DBGS(PRINT_START(""));

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QStandardItem *rootItem = model->invisibleRootItem();

  dataBase.setRootItem(rootItem);
  dataBase.setJotter(jotter);

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

      rv = dataBase.write(dataBasePath);
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

void MainWindow::ksilitSlotOpen() {
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

  initMainWindow();

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
    ui->action_Delete_Note->setEnabled(true);
    ui->actionAdd_Note->setEnabled(true);
    ui->actionMove_Up->setEnabled(true);
    ui->actionMove_Down->setEnabled(true);
  }
  else {
    jotterActionAddNote->setEnabled(false);
    jotterActionDeleteNote->setEnabled(false);
    jotterActionMoveUp->setEnabled(false);
    jotterActionMoveDown->setEnabled(false);
    ui->action_Delete_Note->setEnabled(false);
    ui->actionAdd_Note->setEnabled(false);
    ui->actionMove_Up->setEnabled(false);
    ui->actionMove_Down->setEnabled(false);
  }

  QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->jotterTreeView->model());
  QModelIndex selectedIndex = ui->jotterTreeView->selectionModel()->currentIndex();

  if (selectedIndex.isValid()) {
    QStandardItem *selectedItem = model->itemFromIndex(selectedIndex);
    int selectedRow = selectedItem->row();

    if (selectedRow == 0) {
      jotterActionMoveUp->setEnabled(false);
      ui->actionMove_Up->setEnabled(false);
    }

    QStandardItem *parentItem = selectedItem->parent();

    if (!parentItem) {
      parentItem = model->invisibleRootItem();
    }

    int childCount = parentItem->rowCount();
    int lastRow = childCount - 1;

    if (selectedRow == lastRow) {
      jotterActionMoveDown->setEnabled(false);
      ui->actionMove_Down->setEnabled(false);
    }
  }

  if (isKsilitSaveEnabled()) {
    setWindowModified(true);

    ui->action_Save->setEnabled(true);
  }
  else {
    setWindowModified(false);

    ui->action_Save->setEnabled(false);
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
      rv = dataBase.write(dataBasePath);
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

void MainWindow::initMainWindow() {
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
  rv = dataBase.read(dataBasePath);
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
