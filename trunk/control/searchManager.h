//@Info This class is composed by a metaRank, and is at the center point of View and Model.

#ifndef SEARCHMANAGER_H_
#define SEARCHMANAGER_H_

#include <QMutex>
#include "searchResults.h"
#include "metaRank.h"
#include "tdParam.h"
#include "qHideEvent.h"

class searchManager
{
public:
	searchManager(Ui::MainWindow *ui,QMainWindow *mw);
	virtual ~searchManager();
	void freeSearch();
	void endSearch();
	void newSearch(list<string> &eng,list<string> &engnames, tdParam *t);
  vector<row> & getResults();
  int getResultsNumber(int old = 0);
  int getEngineResults(const string & name);
  void sortResults(string s, bool ascending = true);
  
private:
	metaRank *meta;
	Ui::MainWindow *ui;
  QMainWindow *mw;
  vector<row> emptyVector;
  QMutex mutex;
};

#endif /*SEARCHMANAGER_H_*/
