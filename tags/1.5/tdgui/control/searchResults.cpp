#include "searchResults.h"

searchResults::searchResults(QMainWindow *mw) {
	this->mw = mw;
}

void searchResults::addRankedResults(const vector<row> & r, string engname) {
  engineResults::addRankedResults(r,engname);
  QApplication::postEvent(mw,new QInsertEvent(engname.c_str()));
}

searchResults::~searchResults() {
}
