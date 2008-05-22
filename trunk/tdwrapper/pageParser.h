/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
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
#include "tdParam.h"
#include "tdExcp.h"
#include "threads/threadPool.h"


using namespace std;

class pageParser {

public:
  /**
   * @param e The name of the engine to treat
   * @param ler A pointer to the query parameters. Will alse be used to store the results.
   * @param mut The mutex to use to update the results.
   * @param t the glocal engine parameters (proxies, etc...).
   */
	pageParser(const string & e, engineResults *ler, threadPool<pageParser> *mut, tdParam *t);
	pageParser(const pageParser &);
	~pageParser();
	
	/**
   * This method is used to handle the whole engine parsing.
   */
	void startParsing();
	
	/**
   * A convenience function for threads manageability
   */
	void operator()() { startParsing(); }
	
	/**
	 * Set the global cancellation value.
	 * @param val If set to true the parsing will end as soon as it can without storing its results
	 */
	void setAbort(bool val = true) { global_results->setAbort(val); }
	
  bool getAbort() { return *global_results->getAbort(); }
	vector<row> & getResults();
	string getEngineName() { return engname; }
  
private:
	bool initParse();
	void doParse();
	void closeParse(bool = true);
	row generateRow(regExp &);
	address getNextAddress();
	
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
};

#endif
