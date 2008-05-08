/*
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
		 * @param res This objects contains the query, the list of engines that should be queried and the number of results per engines. It will also be used to store the results.
		 * @param t the tdParam parameters to pass to engines  
		 */
    metaRank(engineResults *res, tdParam *t);
    
		/**
		 * Metarank initializer.
		 * @param res This objects contains the query, the list of engines that should be queried and the number of results per engines. It will also be used to store the results.
		 * @param t the tdParam parameters to pass to engines  
		 */
    void init(engineResults *res, tdParam *t);
    
    ~metaRank();

    /**
     * This method executes the search on the given engines.
     * For each engine, a thread is created, which enables query to be processed in parallel.
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
    @param old if given, it is returned unless more results than old have been found.
    Usefull to get a strictly non decreasing number as result number can decrease
    when ranking is started, due to multiple same results.
    */
    int getResultsNumber(int old = 0);
    
    /**
     * A simple method used to display the final results.
     */
    void toString();
    
    /**
     * A more advanced method to display only required fields
     * @param page The format of a single result with brace enclosed fields name
     * @param d The engine list separators 
     */
    void toString(string page, string d);
    
    /**
     * A more advanced method to display only required fields
     * @param page The format of a single result with brace enclosed fields name
     * @param delim_engs The engine list separator
     * @param delim_res The results separator
     * @param pre A string to add before the output
     * @param post A string to add after the output
     * @param escape If true, HTML escapes the replaced fields 
     */
    string getString(string page, string delim_engs,string delim_res = "", string pre = "", string post = "", bool escape = false );
    
    /**
     * Sort the given list of row in order of the given field
     * @param lr The list of rows
     * @param field The field name
     */
    static void sort(vector<row> & lr,string field);    
    
    /**
     * @param name The name of the engine
     * @return The number of results that have been retrieved for the given engine
     */
    int getEngineResults(const string & name);
    
    /**
     * @return A pointer to the results number map
     */
    map<string,int> *getEngineResults();
    
    
    /**
     * Sort the results
     * @param s The column name to sort
     * @param ascending The direction of sort
     */
    void sortResults(string s, bool ascending = true);
    
    /** This method only returns when new results are available
     * @return true if every awaited results available
     */
    bool waitForNewResults();
    
private:
    threadPool<pageParser> *threads;
    list<pageParser *> pplist;
    engineResults *ranked_results;
    static string intToString(int);
    tdParam *tdp;
};

#endif
