#ifndef TREEMANAGER_H_
#define TREEMANAGER_H_

#include "ui_teardrop.h"
#include "nodeDoc.h"
#include "xmlEngine.h"
#include "selectFile.h"
#include <QStandardItemModel>
#include <qmessagebox.h>
#include <QIcon>

#define COLUMN_NAME 0
#define COLUMN_FILENAME 1

class treeManager
{
public:
	treeManager(Ui::MainWindow *);
	virtual ~treeManager();
	void generateModel();
	void addNewItem(QStandardItem *parent, QStandardItem *child, QIcon *);
	void engineItemChanged(QTreeWidgetItem *it);
	void getListEngines(list<string> & eng, int c = COLUMN_FILENAME);
private:
	QStandardItemModel *treeModel;
	Ui::MainWindow *ui;
};

#endif /*TREEMANAGER_H_*/
