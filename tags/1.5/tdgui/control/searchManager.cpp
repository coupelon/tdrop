#include "searchManager.h"

searchManager::searchManager(Ui::MainWindow *ui,QMainWindow *mw) {
	this->ui = ui;
	//Meta is the meta search variable. It must be NULL so that Teardrop knows it has not yet been used
	meta = NULL;
  this->mw = mw;
}

searchManager::~searchManager() {
	freeSearch();
}

void searchManager::newSearch(list<string> &eng, list<string> &engnames, tdParam *t) {
  searchResults *sr = new searchResults(mw);
	freeSearch(); 
	//ui->progressionBox->setVisible(true);
	for(list<string>::const_iterator it = engnames.begin();
			it != engnames.end(); ++it) {
		QListWidgetItem *item = new QListWidgetItem(it->c_str());
		item->setFlags(Qt::ItemIsEnabled);
		item->setCheckState(Qt::Unchecked);
		ui->progressionList->addItem(item);
	}

	for(list<string>::const_iterator it = eng.begin();
			it != eng.end(); ++it) {
	  sr->addEngine(*it);
 	}
 	
	sr->setQuery(ui->queryEdit->text().toStdString());
	sr->setLimit(ui->spinBox->value());
	
	meta = new metaRank(sr,t);
	meta->startParsing();
}

void searchManager::endSearch() {
	if (meta != NULL) {
		meta->stop();
	}
}

void searchManager::freeSearch() {
	if (meta != NULL) {
		meta->stop();
		mutex.lock();
		ui->progressionList->clear();
		ui->resultsTable->clear();
		delete meta;
		mutex.unlock();
	}
}

vector<row> & searchManager::getResults() {
  if (meta != NULL)
    return meta->getResults();
  else {
    return emptyVector;
  }
}

int searchManager::getResultsNumber(int old) {
  if (meta != NULL)
    return meta->getResultsNumber(old);
  else return 0;
}

int searchManager::getEngineResults(const string & name) {
  if (meta != NULL)
    return meta->getEngineResults(name);
  return 0;
}

void searchManager::sortResults(string s, bool ascending) {
  if (meta != NULL)
    meta->sortResults(s,ascending);
}


