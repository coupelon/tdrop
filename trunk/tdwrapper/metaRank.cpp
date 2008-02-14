/** Teardrop - metaRank.cpp

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
    string p;   
    for (vector<row>::iterator lrit = ranked_results->getResults().begin(); ranked_results->getResults().end() != lrit; ++lrit) {
        p = page;
        list<string> ls = lrit->getEngine();
        string engines = "";
        for(list<string>::iterator lsit = ls.begin(); lsit != ls.end(); ++lsit) {
            if (engines != "") engines += d;
            engines += *lsit;
        }  
        p = regExp::replaceAll(p,"\\x7bnum\\x7d",metaRank::intToString(lrit->getNum()));
        p = regExp::replaceAll(p,"\\x7bengines\\x7d",engines);
        p = regExp::replaceAll(p,"\\\\n","\n");
        p = regExp::replaceAll(p,"\\\\t","\t");

        regExp r("\\x7b([^\\x7d]*)");  // Regexp : {[^}]*}
        
        for(r.newPage(p); !r.endOfMatch(); r.next()) {
            //cout << "_" << r.getMatch(1) << "_" << endl;
            p = regExp::replaceAll(p,"\\x7b" + r.getMatch(1) + "\\x7d",lrit->getFields()[r.getMatch(1)]);
        }
        cout << p;
    }
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

void metaRank::sortResults(string s, bool ascending) {
  if (ranked_results != NULL)
    ranked_results->sortResults(s,ascending);
}

