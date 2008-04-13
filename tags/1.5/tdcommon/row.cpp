/** Teardrop - row.cpp

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

#include "row.h"

row::row(int n) {
	num = n;
}

row::row(int n, string e) {
    num = n;
    engine.push_back(e);
}

int row::getNum() const {
	return num;
}

void row::setNum( int n ) {
	num = n;
}

list<string> row::getEngine() const {
    return engine;
}

string row::getEngineList(int max) const {
  string output;
  int i = 0;
  for(list<string>::const_iterator lsit = engine.begin(); lsit != engine.end(); ++lsit) {
    if (i)
      output += "," + *lsit;
    else 
      output += *lsit;
    if (++i == max) break;
  }
  return output;
}

void row::addField(string f, string n) {
	fields[n] += f;
}

void row::addComparable(string c) {
    comparable.push_back(c);
}

void row::addEngine(string e) {
    engine.push_back(e);
}

void row::addEngine(list<string> e) {
    engine.insert(engine.end(),e.begin(),e.end());
}

void row::toString(bool n) {
	if (n) cout << num;
    for(list<string>::iterator lsit = engine.begin(); lsit != engine.end(); ++lsit) {
        cout << " " << *lsit;
    }
    for(map<string, string, compstr>::iterator msscit = fields.begin(); msscit != fields.end(); ++msscit) {
        if (n || msscit != fields.begin()) cout << endl;
        cout << "\t" << msscit->first << ":" << msscit->second;
	}
	cout << endl;
}

map<string, string, compstr> & row::getFields() {
    return fields;
}

string row::getField(string s) const {
    map<string, string, compstr>::const_iterator fnd  = fields.find(s);
    if (fnd != fields.end()) return fnd->second;
    return "";
}

bool row::operator==(const row & r) const {
    if (comparable.size() != r.comparable.size()) return false;
    map<string, string, compstr>::const_iterator m1;
    map<string, string, compstr>::const_iterator m2;
    for(list<string>::const_iterator lsit = comparable.begin(); lsit != comparable.end(); ++lsit) {
        if (!((m1 = fields.find(*lsit)) != fields.end() &&
              (m2 = r.fields.find(*lsit)) != r.fields.end() &&
               m1->second == m2->second))
            return false;
    }
    return true;
}
