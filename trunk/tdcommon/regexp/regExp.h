/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef REGEXP_H
#define REGEXP_H

#include <iostream>
#include <pcre.h>

#include "../tdExcp.h"

#define VECTORSIZE 30

using namespace std;

class regExp {
public:
	static string replaceAll(string,string,string);
	regExp(string ,string &);
    regExp(string);
	~regExp();
    void newPage(string &);
	void next();
	bool endOfMatch();
	string getMatch(int);
	int getMatchNumber(); 
private:
	pcre *reg;
	const char *error;
        int erroffset;
	string page;
	int ovector[VECTORSIZE];
	int rc;
	int offset;
};

#endif
