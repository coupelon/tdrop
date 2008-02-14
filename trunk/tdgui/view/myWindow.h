#ifndef MYWINDOW_H
#define MYWINDOW_H

class myWindow;

#include "ui_teardrop.h"
#include "treeManager.h"
#include "../control/searchManager.h"
#include "../control/resultsColumns.h"
#include <QApplication>
#include <qmessagebox.h>
#include <QHeaderView>
#include "../control/qInsertEvent.h"
#include "../control/qHideEvent.h"
#include "qResultsDelegate.h"
#include "prefDialog.h"
#include "tdParam.h"
#include <QProcess>

 class myWindow : public QMainWindow
 {
     Q_OBJECT

 public:
     myWindow(QMainWindow* parent = 0);
     ~myWindow();
     
     searchManager *getSManager() { return sManager; }

 private:
     Ui::MainWindow ui;
     treeManager *engineTreeManager;
     searchManager *sManager;
     tdParam param;
     
     virtual void customEvent(QEvent * event);
     
 private slots:
     void doSearch();
     //void doCancelSearch();
     void doAbortSearch();
     void engineItemChanged(QTreeWidgetItem *it,int c);
     void doubleClickedResults(const QModelIndex & it);
     void sortResults(int column, Qt::SortOrder order);
     void showProgression(bool);
     void showPreferences();
     void showAbout();
     void changeChevProgression();
     void changeChevHistory();
     
 };

 #endif

