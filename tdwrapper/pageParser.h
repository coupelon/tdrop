/** Teardrop - pageParser.h

@brief This class provide a way to question a given single search engine

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 04/04/2006

# Copyright (C) 2006 Olivier COUPELON

# This file is part of Teardrop.
#
# Teardrop is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Teardrop is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Teardrop; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <iostream>
#include <list>
#include <set>
#include "row.h"
#include "regexp/regExp.h"
#include "xml/xmlEngine.h"
#include "net/getHttp.h"
#include "address.h"
#include "engineResults.h"
#include "debugmacro.h"
#include "tdParam.h"
#include "tdExcp.h"
#include "threads/threadPool.h"


using namespace std;

class pageParser {
private:
	xmlEngine *eng;
	string query;
	string engname;
	list<address> pages;
	set<address> visited_pages;
	vector<row> results;
	engineResults *global_results;
	threadPool<pageParser> *results_lock;
	unsigned long limit;
	getHttp gh;
    tdParam *tdp;

	bool initParse();
	void doParse();
	void closeParse();
	row generateRow(regExp &);
	address getNextAddress();
public:
	pageParser(const string &, engineResults *, threadPool<pageParser> *, tdParam *);
	pageParser(const pageParser &);
	~pageParser();
	void startParsing();
	void operator()() { startParsing(); }
	vector<row> & getResults();
	string getEngineName() { return engname; }
	void setAbort(bool val = true) { global_results->setAbort(val); }
  bool getAbort() { return *global_results->getAbort(); }
};

#endif
