/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
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
