/** 
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

//Name of the fake field corresponding to the number, for row comparison 
#define ROW_NUMBER "number"

struct compstr {
    bool operator()(const string s1, const string s2) const
    {
        return s1 < s2;
    }
};

class row {
public:
  row(int n = 0);
  row(int, string);
  int getNum() const;
  void setNum(int);
  list<string> getEngine() const;
  void addField(string, string);
  void addComparable(string);
  void addEngine(string);
  void addEngine(list<string>);
  map<string, string, compstr> & getFields();
  string getField(string s) const;
  void toString(bool n = true);
  bool operator==(const row &) const;
  string getEngineList(int max = -1) const;

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
      string s1 = r1.getFields()[field];
      transform(s1.begin(),s1.end(),s1.begin(),(int (*)(int)) toupper);
      string s2 = r2.getFields()[field];
      transform(s2.begin(),s2.end(),s2.begin(),(int (*)(int)) toupper);
      return s1 < s2;
      //return r1.getFields()[field] < r2.getFields()[field];
    }
    bool num;
    string field;
};

#endif
