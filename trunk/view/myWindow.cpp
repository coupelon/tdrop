#include "myWindow.h"

myWindow::myWindow(QMainWindow *parent) : QMainWindow(parent) {
  ui.setupUi(this);
  //ui.progressionBox->layout()->setSpacing(0);
  
  //ui.progressionBox->setVisible(false);
  
  // This part resizes the splitters so that the results view is the biggest widgets.
  // There must be a better way to do that, but i didn't figured it out yet !
  QList<int> sizes_h,sizes_w;
  sizes_h << 0 << 1024;
  sizes_w << 250 << 1024;
  ui.splitter->setSizes(sizes_h);
  ui.splitter_2->setSizes(sizes_w);
  ui.splitter->setStretchFactor(1, 1);
  ui.splitter_2->setStretchFactor(1, 1);
  
  //Set results column labels :
  ui.resultsTable->setColumnCount(RESULT_COLUMN_COUNT);
  QStringList headers;
  headers << RESULT_COLUMN_NAME;
  ui.resultsTable->setHeaderLabels(headers);
  /*for (int i = RESULT_COLUMN_VISIBLE; i < RESULT_COLUMN_COUNT; ++i) {
    ui.resultsTable->setColumnHidden(i,true);
  }
  ui.resultsTable->header()->setStretchLastSection(false);
  ui.resultsTable->header()->setResizeMode(QHeaderView::Stretch);*/
  ui.resultsTable->setItemDelegate(new qResultsDelegate(this,&param));
  ui.resultsTable->setSelectionBehavior( QAbstractItemView::SelectRows );

  ui.resultsTable->header()->setVisible(false);
  
  // Populates the engine tree widget.
  engineTreeManager = new treeManager(&ui);
  
  sManager = new searchManager(&ui,this);
 
  // Connects the required functions.
  connect(ui.searchButton, SIGNAL(clicked()), this, SLOT(doSearch()));
  //connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(doCancelSearch()));
  connect(ui.resultsTable->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(sortResults(int, Qt::SortOrder)));
  connect(ui.abortButton, SIGNAL(clicked()), this, SLOT(doAbortSearch()));
  connect(ui.engineTreeView, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(engineItemChanged(QTreeWidgetItem *,int)));
  connect(ui.resultsTable, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClickedResults(const QModelIndex &)));
  //connect(ui.progressionBox, SIGNAL(toggled(bool)), this, SLOT(showProgression(bool)));
  connect(ui.actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
  connect(ui.action_About, SIGNAL(triggered()), this, SLOT(showAbout()));
  connect(ui.chevButtonProgression, SIGNAL(clicked()), this, SLOT(changeChevProgression()));
  connect(ui.chevButtonHistory, SIGNAL(clicked()), this, SLOT(changeChevHistory()));
}

myWindow::~myWindow() {
	delete engineTreeManager;
	delete sManager;
}

void myWindow::doSearch() {
  if (ui.queryEdit->text().isEmpty())
    (void) QMessageBox::information(this, tr("No Search specified"),
      tr("Please supply a search."), QMessageBox::Ok);
  else {
    //Do the search.
    list<string> engines;
    engineTreeManager->getListEngines(engines);
    if (engines.empty())
    	(void) QMessageBox::information(this, tr("No Engines select"),
     		tr("Please select at least one engine."), QMessageBox::Ok);
  	else {
  	  ui.resultsTable->clear();
  		list<string> engnames;
  		engineTreeManager->getListEngines(engnames,COLUMN_NAME); 
  		sManager->newSearch(engines,engnames,&param);
  	}
  }
}

void myWindow::doAbortSearch() {
  sManager->endSearch();
}

// Callback function called when an engine item was modified.
void myWindow::engineItemChanged(QTreeWidgetItem *it, int c) {
	engineTreeManager->engineItemChanged(it);
}

//Receives the custom events
void myWindow::customEvent(QEvent * event) {
  if (event->type() == QInsertEventNumber) {
    int items = sManager->getResultsNumber();
    //If there are too much items in the list, remove some
    /*while (items <  ui.resultsTable->topLevelItemCount())
      ui.resultsTable->takeTopLevelItem(0);*/
    ui.resultsTable->clear();
    //If there are not enough items in the list, add some
    while (items > ui.resultsTable->topLevelItemCount()) {
      QTreeWidgetItem *qtit = new QTreeWidgetItem();
      ui.resultsTable->insertTopLevelItem(ui.resultsTable->topLevelItemCount(),qtit);
      ui.resultsTable->setFirstItemColumnSpanned(qtit,true);
      //ui.resultsTable->setFirstColumnSpanned(ui.resultsTable->topLevelItemCount()-1,ui.resultsTable->rootIndex(),true);
    }
    ui.progressionList->findItems(((QInsertEvent *) event)->getEngine(),Qt::MatchExactly).front()->setCheckState(Qt::Checked);
    QString newname = ((QInsertEvent *) event)->getEngine() + " (" + QString::number(sManager->getEngineResults(((QInsertEvent *) event)->getEngine().toStdString())) + ")";
    ui.progressionList->findItems(((QInsertEvent *) event)->getEngine(),Qt::MatchExactly).front()->setText(newname);
  /*} else if (event->type() == QHideEventNumber) {
    ui.progressionBox->setVisible(false);*/
  } else cerr << "Unknown event received " << event->type() << endl;
}

void myWindow::doubleClickedResults(const QModelIndex & it) {
  QString mailApp = "firefox";
  QStringList param;
  param << sManager->getResults()[it.row()].getField("url").c_str();
  QProcess* process = new QProcess(this);
  process->start(mailApp , param);
  //cout << "double clicked in Teardrop" << endl;
}

void myWindow::sortResults(int column, Qt::SortOrder order) {
  bool asc = (order == Qt::AscendingOrder);
  if (column == 0)
    sManager->sortResults(ROW_NUMBER,!asc);
  else if (column == 1)
    sManager->sortResults("title",asc);
  else if (column == 2)
    sManager->sortResults("abstract",asc);
  else if (column == 3)
    sManager->sortResults("url",asc);
}

void myWindow::showProgression(bool on) {
  ui.progressionList->setVisible(on);
  ui.abortButton->setVisible(on);
}

void myWindow::showPreferences() {
  prefDialog *pd = new prefDialog(&param);
  pd->exec();
  delete pd;
}

void myWindow::showAbout()
{
	QMessageBox::about(this, "About Teardrop...",
		"Thank you for using Teardrop !\n\n" \
		"Developed by Olivier Coupelon (olivier.coupelon@wanadoo.fr)\n" \
		"Licenced under GPL\n");
}

void myWindow::changeChevProgression() {
  if (ui.frameProgression->isVisible()) {
    ui.frameProgression->setVisible(false);
    ui.chevButtonProgression->setIcon(QIcon(QString::fromUtf8(":/images/icons/arrow-down-double.png")));
  } else {
    ui.frameProgression->setVisible(true);
    ui.chevButtonProgression->setIcon(QIcon(QString::fromUtf8(":/images/icons/arrow-up-double.png")));
  }
}

void myWindow::changeChevHistory() {
  if (ui.historyTable->isVisible()) {
    ui.historyTable->setVisible(false);
    ui.chevButtonHistory->setIcon(QIcon(QString::fromUtf8(":/images/icons/arrow-down-double.png")));
  } else {
    ui.historyTable->setVisible(true);
    ui.chevButtonHistory->setIcon(QIcon(QString::fromUtf8(":/images/icons/arrow-up-double.png")));
  }
}

