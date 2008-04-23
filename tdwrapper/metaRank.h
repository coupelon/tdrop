/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef METARANK_H
#define METARANK_H

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "threads/threadPool.h"
#include "pageParser.h"
#include "row.h"
#include "engineResults.h"
#include "tdParam.h"

using namespace std;

class metaRank {
public:
	/**
	 * Metarank initializer.
	 * @param query The request
	 * @param e The list of engines that should be queried
	 * @param limit The number of results per engines
	 * @param t the tdParam parameters to pass to engines
	 * @param res A pointer to a engineResults instance aimed to contain the results. The instance will be deleted by Teardrop.  
	 * */
    metaRank(engineResults *res, tdParam *t);
    void init(engineResults *res, tdParam *t);
    ~metaRank();

    /** This method executes the search on the given engines.
    For each engine, a thread is created, which enables query to be
    processed in parallel.
    */   
    void startParsing();

    /** Wait for all the pageParser to finish
    */
    void joinAll();

    /** The list of results finaly obtained
    */
    vector<row> & getResults();
    
    /** Stops the current threads by cancelling them.
    */
    void stop();
        
    /** The number of results obtained so far.
    @param if given, it is returned unless more results than old have been found.
    Usefull to get a strictly non decreasing number as result number can decrease
    when ranking is started, due to multiple same results.
    */
    int getResultsNumber(int old = 0);
    
    /** A simple method used to display the final results.
    */
    void toString();
    
    /** A more advanced method to display only required fields   
    */
    void toString(string, string);
    string getString(string, string, string = "", string = "", string = "", bool = false);
    
    /** Sort the given list of row in order of the given field
    */
    static void sort(vector<row> & lr,string field);    
    
    /** Return the number of results that have been retrieved for the given engine
     */
    int getEngineResults(const string & name);
    
    /** Retunrs a pointer to the results number map
     */
    map<string,int> *getEngineResults();
    
    
    /** Sort the results by the given column (s), in the given order (ascending)
     */
    void sortResults(string s, bool ascending = true);
    
    /** This method only returns when new results are available
     * Returns true if every awaited results available
     */
    bool waitForNewResults();
    
private:
    threadPool<pageParser> threads;
    list<pageParser *> pplist;
    engineResults *ranked_results;
    static string intToString(int);
    tdParam *tdp;
};

#endif
