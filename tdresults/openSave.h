/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef OPENSAVE_H
#define OPENSAVE_H

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <sstream>
#include "row.h"
#include "net/getHttp.h"
#include "xml/xmlFile.h"
#include "regexp/regExp.h"
#include "metaRank.h"
using namespace std;

class openSave {
public:
    /*
     * This method saves a query in a file.
     * @param file The file + path to use
     * @param query The query used
     * @param engines The engine list used for that query
     * @param limit The number of results per engines
     * @param r The array of results 
     */
    static bool xmlSave(string file, string query, map<string, int> engines, unsigned long limit, vector<row> r);
    
    /**
     * This method saves a query in a file.
     * @param file The file + path to use
     * @param the metaRank object
     */
    static bool xmlSave(string file, metaRank *mr);
    
	/**
	 * This method loads a previously saved file.
     * @param file The file + path to use
     * @param mr a new metaRank object
     * @param tdp the tdParam to use for the new metaRank
	 */
	static bool xmlOpen(const string & file, metaRank **mr, tdParam *tdp);

    /**
     * This method saves a query in a file (HTML).
     * @param myfile The html container
     * @param the metaRank object to extract the results from
     */
    static void htmlExport(stringstream & myfile, metaRank *mr);
    
    /**
     * This method saves a query in a file (HTML).
     * @param file The file + path to use
     * @param the metaRank object to extract the results from
     */
    static bool htmlExport(string & file, metaRank *mr);

    /*
     * This method saves a query in a file (CSV).
     * @param mr the metaRank object containing the results
     */
    static string csvExport(metaRank *mr);
    
    /*
     * This method saves a query in a file (CSV).
     * @param file The file + path to use
     * @param r The array of results 
     */
    static bool csvExport(string file, const vector<row> & r);
    
    /*
     * This method saves a query in a file (CSV).
     * @param myfile The file container
     * @param r The array of results 
     */
    static void csvExport(stringstream & myfile, const vector<row> & r);
    
};

#endif
