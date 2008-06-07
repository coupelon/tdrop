/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef UIDSESSION_H_
#define UIDSESSION_H_

#include <iostream>
#include <ctime> 
#include <cstdlib>
#include "metaRank.h"

using namespace std;

/**
 * This class is designed to manage the generation of unique identifiers
 * for the web server session management. Those identifiers are composed
 * of 29 alphanumerical characters, and returned as strings.
 */

class UIDSession
{
public:
	UIDSession();
	virtual ~UIDSession();
	static string getID();
	static void initRand();
	string addSearch(string userID, metaRank *mr);
	metaRank *getSearch(string userID, string searchID);
private:
	struct clientSearch {
		string userID;
		metaRank *mr;
	};
	map<string, clientSearch> *searches;
};

#endif /*UIDSESSION_H_*/
