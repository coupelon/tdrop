/** Teardrop - metaRank.h

@brief This file is designed to launch multiple engines query via threads.
@brief It also gives a way to sort the results.

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
    
    /** Sort the given list of row in order of the given field
    */
    static void sort(vector<row> & lr,string field);    
    
    /** Return the number of results that have been retrieved for the given engine
     */
    int getEngineResults(const string & name);
    
    
    /** Sort the results by the given column (s), in the given order (ascending)
     */
    void sortResults(string s, bool ascending = true);
    
private:
    threadPool threads;
    list<pageParser *> pplist;
    engineResults *ranked_results;
    static string intToString(int);
    tdParam *tdp;
};

#endif
