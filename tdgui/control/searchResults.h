#ifndef SEARCHRESULTS_H_
#define SEARCHRESULTS_H_

#include "ui_teardrop.h"
#include "engineResults.h"
#include "row.h"
#include "resultsColumns.h"
#include "qInsertEvent.h"

class searchResults : public engineResults
{
public:
	searchResults(QMainWindow *);
	virtual ~searchResults();
	
	virtual void addRankedResults(const vector<row> & r, string engname);
	
private:
	QMainWindow *mw;
};

#endif /*SEARCHRESULTS_H_*/
