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
#include "tdParam.h"
#include "openSave.h"

using namespace std;

/**
 * This class is designed to manage the generation of unique identifiers
 * for the web server session management. Those identifiers are composed
 * of 29 alphanumerical characters, and returned as strings.
 */

class UIDSession
{
public:
	UIDSession(tdParam *tdp);
	virtual ~UIDSession();
	
	/**
	 * Generate a new unique ID. The uniqueness property is assumed
	 * due to the ramdomness of the ID generation.
	 * @return the ramdom unique ID
	 */
	static string getID();
	
	/**
	 * Initialize the random number generator.
	 * Here we should be using some kind of mersene twister random
	 * number generator, but the default one is considered sufficient
	 * as of now.
	 */
	static void initRand();
	
	/**
	 * Add a new search to the list, and generate its ID.
	 * @param usedID The Cookie of the current user
	 * @param mr The metaRank search
	 * @return the search new ID 
	 */
	string addSearch(string userID, metaRank *mr, string userName);
	
	/**
	 * This method returns the metaRank corresponding the the given
	 * search ID, and ensures that the user is correct.
	 * @return the search metaRank
	 */
	metaRank *getSearch(string userID, string searchID, string userName);
	
	/**
	 * This method stores currently in memory terminated searches
	 * to disk, as sessions searches. 
	 */
	void dumpToDisk();
private:
	struct clientSearch {
		string userID;
		string userName;
		metaRank *mr;
	};
	tdParam *tdp;
	map<string, clientSearch> *searches;
};

#endif /*UIDSESSION_H_*/
