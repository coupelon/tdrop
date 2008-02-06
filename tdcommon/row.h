/** Teardrop - row.h

@brief This class is used to represent a row (tuple) extracted by a query.

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 04/04/2006

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
