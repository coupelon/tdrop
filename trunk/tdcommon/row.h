/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef ROW_H
#define ROW_H

#include <iostream>
#include <map>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

///Name of the fake field corresponding to the number, for row comparison 
#define ROW_NUMBER "number"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct compstr {
    bool operator()(const string s1, const string s2) const
    {
        return s1 < s2;
    }
};
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

class row {
public:
	/**
	 * Creates a new row
	 * @param n The row number (Defaults to 0)
	 */
  row(int n = 0);
  
  /**
	 * Creates a new row
	 * @param n The row number (Defaults to 0)
	 * @param e The engine that retrieved that row
	 */
  row(int n, string e);
  
  /**
   * Add a field (i.e. a column) to that row
   * @param n the field name
   * @param f the field value
   */
  void addField(string f, string n);
  
  /**
   * Set a field as comparable
   * @param c The field name 
   */
  void addComparable(string c);
  
  /**
   * Add an engine to that row
   * @param e The engine name 
   */
  void addEngine(const string & e);
  
  /**
   * Add a list of engines to that row
   * @param e The engines name list 
   */
  void addEngine(const list<string> & e);
  
  /**
   * Retrieve the engine list in a comma separated string of engines names
   * @param max The maximun number of engines to retrieve
   * @return A comma separated list of engines
   */
  string getEngineList(int max = -1) const;
  
  /**
   * Output that row as a string to stdout 
   */
  void toString(bool n = true);
  
  int getNum() const;
  void setNum(int);
  const list<string> & getEngine() const;
  const map<string, string, compstr> & getFields() const;
  string getField(string s) const;
  
  bool operator==(const row &) const;

private:
  int num;
  list<string> engine;
  list<string> comparable;
  map<string, string, compstr> fields;
};

struct compareRow {
    compareRow(const string & s) {
    	if (s == ROW_NUMBER) num = true;
    	else num = false;
    	field = s;
    }
    bool operator()(/*const */row r1, /*const */row r2) const {
      if (num) return r1.getNum() < r2.getNum();
      string s1 = r1.getField(field);
      transform(s1.begin(),s1.end(),s1.begin(),(int (*)(int)) toupper);
      string s2 = r2.getField(field);
      transform(s2.begin(),s2.end(),s2.begin(),(int (*)(int)) toupper);
      return s1 < s2;
      //return r1.getFields()[field] < r2.getFields()[field];
    }
    bool num;
    string field;
};

#endif
