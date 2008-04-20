/** Teardrop - engineResults.h

@brief This class is more like a struct. It's just a
@brief collection of information usefull for debugging
@brief and simplifying the code.

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 07/08/2006

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

#ifndef ENGINERESULTS_H
#define ENGINERESULTS_H

#include <iostream>
#include <vector>
#include "row.h"
#include "threads/threadPool.h"

using namespace std;

class engineResults {
public:
	engineResults() {
		threadPool<engineResults>::lock(new_results);
	  abort = false;
	};
	virtual ~engineResults() {};

  unsigned long & getLimit() { return limit; }
  map<string, int> & getEngines() { return engines; }
  string & getQuery() { return query; }
  map<string, string> & getOptions() { return options; }
  void setLimit(unsigned long l) { limit = l; }
  void setQuery(string q) { query = q; }
  void addEngine(string e) { engines[e] = -1; }
  void addOption(string name, string value) { options[name] = value; }
  
	vector<row> & getResults();
	void setResults(const vector<row> & r);
	virtual void addRankedResults(const vector<row> & r, string engname, string sname = "");
	
	int getEngineResults(const string & name);
	map<string,int> & getEngineResults();
	void setAbort(bool val = true) { abort = val; }
	bool *getAbort() { return &abort; }
	void sortResults(string s, bool ascending = true);
	bool waitForNewResults();
	bool everyResultsReceived();
	
	void toString();
protected:
  unsigned long limit;
	map<string,int> engines;
	map<string, string> options;
	string query;
	vector<row> results;
	bool abort;
	threadPool<engineResults> new_results;
};

#endif
