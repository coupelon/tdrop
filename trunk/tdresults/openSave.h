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
#include "row.h"
#include "net/getHttp.h"
#include "xml/xmlFile.h"
#include "regexp/regExp.h"
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
    static bool xmlSave(string file, string query, list<string> engines, string limit, vector<row> r);
    
    /*
     * This method loads a previously saved file.
     * @param file The file + path to use
     * @param query The query used
     * @param engines The engine list used for that query
     * @param limit The number of results per engines
     * @param r The array of results 
     */
    static bool xmlOpen(string file, string & query, list<string> & engines, string & limit, vector<row> & r);

    /*
     * This method saves a query in a file (HTML).
     * @param file The file + path to use
     * @param query The query used
     * @param engines The engine list used for that query
     * @param limit The number of results per engines
     * @param r The array of results 
     */
    static bool htmlExport(string file, string query, list<string> engines, string limit, vector<row> r);

    /*
     * This method saves a query in a file (CSV).
     * @param file The file + path to use
     * @param r The array of results 
     */
    static bool csvExport(string file, vector<row> r);
};

#endif
