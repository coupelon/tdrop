/*
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
	  //This shouldn't be used to create threads, only mutex, so
	  // the maximum number of threads is set to 0.
	  new_results = new threadPool<engineResults>(0);
		threadPool<engineResults>::lock(new_results);
	  abort = false;
	};
	virtual ~engineResults() {
		delete new_results;
	};

  /**
   * Set the maximum number of results per engines
   * @param l The maximum number of results per engines
   */
  void setLimit(unsigned long l) { limit = l; }
  
  /**
   * Set the query
   * @param q The query string
   */
  void setQuery(string q) { query = q; }
  
  /**
   * Add a new engine to the list, which will be used during the search
   * The engine total results number is set to -1, meaning that no results
   * were yet retrieved 
   * @param e The engine name. 
   */
  void addEngine(string e, int qte = -1) { engines[e] = qte; }
  
  /**
   * Add a new option to this search
   * @param name The option name
   * @param value The option value
   */
  void addOption(string name, string value) { options[name] = value; }
  
  /**
   * Add a list of retrieved results to that engineResults
   * @param r The list of rows
   * @param engname The engine the results were retrieved from
   * @param sname The engine raw name, as it was added by addEngine()
   */
	virtual void addRankedResults(const vector<row> & r, string sname = "");
	
	/**
	 * Get the number of results already retrieved from an engine
	 * @param name The engine name
	 * @return The number of results for that engine. -1 means that engine was not used yet. 
	 */
	int getEngineResults(const string & name);
	
	/**
	 * Helps in cancelling the search.
	 * @param val If true, the processing should stop as soon as possible
	 */
	void setAbort(bool val = true) { abort = val; }
	
	/**
	 * Sorts the containing rows
	 * @param s The field name that should be sorted
	 * @param ascending If true, sort the results in ascending order, descending otherwise
	 */
	void sortResults(string s, bool ascending = true);
	
	/**
	 * Uses a mutex to wait for new results.
	 */
	bool waitForNewResults();
	
	/**
	 * Should be called to know weither all engines have been processed
	 * @return True if every results have been processed
	 */
	bool everyResultsReceived();
	
	unsigned long & getLimit() { return limit; }
  	map<string, int> & getEngines() { return engines; }
  	const string & getQuery() const { return query; }
  	map<string, string> & getOptions() { return options; }
  	vector<row> & getResults();
	void setResults(const vector<row> & r);
	map<string,int> & getEngineResults();
	bool *getAbort() { return &abort; }
	
	/**
	 * Output that instance to stdout
	 */
	void toString();
protected:
  	unsigned long limit;
	map<string,int> engines;
	map<string, string> options;
	string query;
	vector<row> results;
	bool abort;
	threadPool<engineResults> *new_results;
};

#endif
