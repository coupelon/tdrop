/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
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

map<string,int> & engineResults::getEngineResults() {
  return engines;
}

void engineResults::addRankedResults(const vector<row> & r,string engname, string sname) {
  engines[sname] = r.size();
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
	threadPool<engineResults>::unlock(new_results);
}

void engineResults::sortResults(string s, bool ascending) {
  if (ascending)
    sort(results.begin(), results.end(), compareRow(s));
  else 
    sort(results.rbegin(), results.rend(), compareRow(s));
}

bool engineResults::waitForNewResults() {
	if (everyResultsReceived()) return true;
	threadPool<engineResults>::lock(new_results);
	return false;
}

bool engineResults::everyResultsReceived() {
	for(map<string,int>::iterator it = engines.begin(); it != engines.end(); ++it) {
		if (it->second == -1) return false;
	}
	return true;
}
