#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>

#include "jotter.h"
#include "database.h"

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
  void ksilitSlotFileSaveAs();
  void ksilitSlotFileOpen();
  void ksilitSlotMenuEditUpdate();
  void ksilitSlotFileSave();
  void ksilitSlotFileNew();
  void ksilitSlotTabChanged(const int &index);
  void jotterContextMenuRequest(const QPoint &position);
  void jotterSlotAddNote();
  void jotterSlotAddSubNote();
  void jotterSlotItemRenamed();
  void jotterSlotDeleteNote();
  void jotterSlotMoveUp();
  void jotterSlotRowChanged(const QModelIndex &current, const QModelIndex &previousIndex);
  void jotterSlotMoveDown();
  void jotterSlotTextChanged();

private:
  void allocateActions();
  void initActions();
  void createConnections();
  void setupModel();
  void dataBaseInit();
  int dataBaseSaveDialog();
  void setJotterRenameEnabled(bool flag);
  void setKsilitSaveEnabled(bool flag);
  bool isJotterRenameEnabled();
  bool isKsilitSaveEnabled();
  void updateActions();
  void ksilitReset();
  void setJotterTextChanged(bool flag);
  int jotterSaveCurrentText();
  bool isJotterTextChangingEnabled();
  void setJotterTextChangingEnabled(bool flag);
  void initMainWindowMenuFile();
  void updateMainWindowMenuEdit();
  void initMainWindowMenuHelp();
  void updateMainWindowMenuBar();
  void updateMainWindowTitle();
  void initMainWindow();
  void initSettings();
  void setSetting(const QString &category, const QString &setting, const QVariant &value);
  QVariant getSetting(const QString &category, const QString &setting);
  int ksilitLoadDataBase();
  QMessageBox::StandardButton saveRequest();
  QMessageBox::StandardButton jotterDeleteRequest();

  void closeEvent(QCloseEvent *event);

  Ui::MainWindow *ui;
  Jotter *jotter;
  QString dataBasePath;
  DataBase dataBase;
  QSettings *settings;

  QAction *ksilitActionFileNew;
  QAction *ksilitActionFileOpen;
  QAction *ksilitActionFileSave;
  QAction *ksilitActionFileSaveAs;
  QAction *ksilitActionFileQuit;
  QAction *ksilitActionHelpAbout;

  QAction *jotterActionAddNote;
  QAction *jotterActionAddSubNote;
  QAction *jotterActionMoveUp;
  QAction *jotterActionMoveDown;
  QAction *jotterActionDeleteNote;

  bool flagKsilitSaveEnabled;
  bool flagJotterRenameEnabled;
  bool flagJotterTextChanged;
  bool flagJotterTextChangingEnabled;
};

#endif // MAINWINDOW_H
