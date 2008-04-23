/** 
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
    //This method saves a query in a file.
    static bool xmlSave(string, string, list<string>, string, vector<row>);
    
    //This method loads a previously saved file.
    static bool xmlOpen(string, string &, list<string> &, string &, vector<row> &);
    
    //This method saves a query in a file (HTML).
    static bool htmlExport(string, string, list<string>, string, vector<row>);
    
    //This method saves a query in a file (CSV).
    static bool csvExport(string file, vector<row>);
};

#endif
