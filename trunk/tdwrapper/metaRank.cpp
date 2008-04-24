/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "metaRank.h"

metaRank::metaRank(engineResults *res,tdParam *t) {
	init(res,t);
}

void metaRank::init(engineResults *res,tdParam *t) {
  ranked_results = res;
  tdp = t;
}

metaRank::~metaRank() {
    delete ranked_results;
}

void metaRank::startParsing() {
    pageParser *p;
    for(map<string,int>::const_iterator eng = ranked_results->getEngines().begin(); ranked_results->getEngines().end() != eng; ++eng) {
        p = new pageParser(eng->first,ranked_results, &threads, tdp);
        pplist.push_back(p);
        threads.createThread(&(*p));
    }
}

void metaRank::joinAll() {
  threads.joinAll();
}

void metaRank::stop() {
  for(list<pageParser *>::iterator ppit = pplist.begin(); ppit != pplist.end(); ++ppit)
    (*ppit)->setAbort();
}

void metaRank::toString() {
  for (vector<row>::iterator lrit = ranked_results->getResults().begin(); ranked_results->getResults().end() != lrit; ++lrit) {
    lrit->toString();
  }
}

void metaRank::toString(string page, string d) {
    cout << getString(page,d);
}

string metaRank::getString(string page, string delim_engs,string delim_res, string pre, string post, bool escape) {
		string out = "";
    string p;   
    for (vector<row>::iterator lrit = ranked_results->getResults().begin(); ranked_results->getResults().end() != lrit; ++lrit) {
        p = page;
        list<string> ls = lrit->getEngine();
        string engines = "";
        for(list<string>::iterator lsit = ls.begin(); lsit != ls.end(); ++lsit) {
            if (engines != "") engines += delim_engs;
            engines += *lsit;
        }  
        p = regExp::replaceAll(p,"\\x7bnum\\x7d",metaRank::intToString(lrit->getNum()));
        p = regExp::replaceAll(p,"\\x7bengines\\x7d",engines);
        p = regExp::replaceAll(p,"\\\\n","\n"); // Replace \n by a line jump
        p = regExp::replaceAll(p,"\\\\t","\t"); // Replace \t by a tab

        regExp r("\\x7b([^\\x7d]*)");  // Regexp : {[^}]*}
        
        getHttp gh; //Used for escaping in HTML
        
        for(r.newPage(p); !r.endOfMatch(); r.next()) {
            //cout << "_" << r.getMatch(1) << "_" << endl;
            string field;
            if (escape) field = gh.escape(lrit->getFields()[r.getMatch(1)]);
            else field = lrit->getFields()[r.getMatch(1)];
            p = regExp::replaceAll(p,"\\x7b" + r.getMatch(1) + "\\x7d",field);
        }
        out += ((out!="")?delim_res:"") + p;
    }
    return pre + out + post;
}

vector<row> & metaRank::getResults() {
    return ranked_results->getResults();
}

int metaRank::getResultsNumber(int old) {
    int nw = ranked_results->getResults().size();
    if (nw > old) return nw;
    return old;
}

string metaRank::intToString(int x) {
    std::ostringstream o;
    o << x;
    return o.str();
}

void metaRank::sort(vector<row> & lr,string field) {
    std::sort(lr.begin(), lr.end(), compareRow(field));
}

int metaRank::getEngineResults(const string & name) {
  if (ranked_results != NULL)
    return ranked_results->getEngineResults(name);
  return 0;
}

map<string,int> *metaRank::getEngineResults() {
	if (ranked_results != NULL)
  	return &(ranked_results->getEngineResults());
  return NULL;
}

void metaRank::sortResults(string s, bool ascending) {
  if (ranked_results != NULL)
    ranked_results->sortResults(s,ascending);
}

bool metaRank::waitForNewResults(){
	if (ranked_results->waitForNewResults()) {
		joinAll();
		return true;
	}
	return false;
}
