/** Teardrop - engineResults.cpp

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

#include "engineResults.h"

vector<row> & engineResults::getResults() {
	return results;
}

void engineResults::setResults(const vector<row> & r) {
	results = r;
}

void engineResults::toString() {
	cout << "Query: " << query << " | limit: " << limit << endl;
	for(vector<row>::iterator it = results.begin(); it != results.end(); ++it) {
		it->toString();
	}
}

int engineResults::getEngineResults(const string & name) {
  return engines[name];
}

void engineResults::addRankedResults(const vector<row> & r,string engname) {
  engines[engname] = r.size();
	//for each row in the input list
	for(vector<row>::const_iterator input = r.begin(); input != r.end(); ++input) {
		//If row is present in the original list
		vector<row>::iterator orig;
		if ((orig = find(results.begin(), results.end(), *input)) != results.end()) {
			//If row is better classified
			if (orig->getNum() > input->getNum()) {
				//Remove original row
				//Add new row, keeping both engines names
				list<string> engs = orig->getEngine();
				*orig = *input;
				orig->addEngine(engs);
			} else {
				//Else add the engine name to original
				orig->addEngine(input->getEngine());
			}
		} else {
			//Else Add new row
			results.push_back(*input);
		}
	}
	//Sort results by num
	sortResults(ROW_NUMBER);
}

void engineResults::sortResults(string s, bool ascending) {
  if (ascending)
    sort(results.begin(), results.end(), compareRow(s));
  else 
    sort(results.rbegin(), results.rend(), compareRow(s));
}

