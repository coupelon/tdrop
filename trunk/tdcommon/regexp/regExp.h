/*
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

/**
 * This class is a frontend to pcre.
 */

class regExp {
public:
	/**
	 * Replaces every occurrences of a string by another
	 * @param s The string in which we want to replace
	 * @param search The regular expression that should be found
	 * @param replace The string we want put instead
	 */
	static string replaceAll(string s,string search,string replace);
	
	/**
	 * The regular expression constructor
	 * @param v The regular expression
	 * @param p The string we want to apply the regular expression to
	 */
	regExp(string v,string & p);
	
	/**
	 * The regular expression constructor
	 * @param v The regular expression
	 */
  regExp(string v);
	~regExp();
	
	/**
	 * Apply the existing regular expression to a new string
	 * @param p The string we want to apply the regular expression to
	 */
  void newPage(string &p);
  
  /**
   * Goes to the next matching part of the string
   */
	void next();
	
	/**
	 * @return true if there are no more matchings in the current string
	 */
	bool endOfMatch();
	
	/**
	 * @param The group number (0 for the whole match). 
	 * @return The value of the given regular expression group for the current matching 
	 */
	string getMatch(int);
	
	/**
	 * Get the number of groups that matched in last execution
	 */
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
