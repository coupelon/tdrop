/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "regExp.h"

regExp::regExp(string v,string & p) {
	page = p;
	offset = 0;
	reg = pcre_compile(
		v.c_str(),          /* the pattern */
		PCRE_DOTALL,                /* default options */
		&error,           /* for error message */
		&erroffset,       /* for error offset */
		NULL);            /* use default character tables */

        next();
}

regExp::regExp(string v) {
    page = "";
    offset = 0;
    reg = pcre_compile(
            v.c_str(),          /* the pattern */
    PCRE_DOTALL,                /* default options */
    &error,           /* for error message */
    &erroffset,       /* for error offset */
    NULL);            /* use default character tables */
}

regExp::~regExp() {
}

void regExp::newPage(string & p) {
    page = p;
    offset = erroffset = 0;
    next();
}

void regExp::next() {
	rc = pcre_exec(
		reg,             /* result of pcre_compile() */
		NULL,           /* we didn't study the pattern */
		page.c_str(),  /* the subject string */
		page.size(),             /* the length of the subject string */
		offset,              /* start at offset 0 in the subject */
		0,              /* default options */
		ovector,        /* vector of integers for substring information */
		VECTORSIZE);            /* number of elements (NOT size in bytes) */
	
	//cerr << "Next: " << offset << " " << page.size() << " " << rc << endl;
	
	if (rc < -1) {
        throw tdExcp("Malformed regular expression.");
		erroffset = 1;
	}
	else offset = ovector[1];
}

bool regExp::endOfMatch() {
	//cerr << "EOF: "<< erroffset << " " << offset << ">=" << page.size() << " " << rc << endl;
	if (erroffset != 0) throw tdExcp("Malformed regular expression.");
    if ((offset > (long) page.size()) || (erroffset) || (rc < 0)) return true;
	return false;
}

string regExp::getMatch(int i) {
	//cerr << "GetMatch: "<< rc << " " << i << " " << ovector[i*2] << " " << ovector[i*2+1]-ovector[i*2] << " " << page.size() << endl;
	//cerr << "submatch : " << page.substr(ovector[i*2],ovector[i*2+1]-ovector[i*2]) << endl;
    if (ovector[i*2] == -1) return "";
	return page.substr(ovector[i*2],ovector[i*2+1]-ovector[i*2]);
}

int regExp::getMatchNumber() {
	return rc - 1;
}

/*string regExp::replaceAll(string s, string search, string replace) {
	string::size_type pos;
	while ((pos = s.find (search,0)) != string::npos) {
		s = s.replace(pos,search.size(),replace);
	}
	return s;
}*/

string regExp::replaceAll(string s, string search, string replace) {
    regExp ra(search,s);
    string::size_type pos = 0;
    while(!ra.endOfMatch()) {
        /*s = s.replace(s.find(ra.getMatch(0),0),ra.getMatch(0).size(),replace);
        ra.next();*/      
        pos = s.find(ra.getMatch(0),pos);
        s = s.replace(pos,ra.getMatch(0).size(),replace);
        pos += replace.size();
        ra.next();
    }
    return s;
}
