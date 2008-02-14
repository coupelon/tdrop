/** Teardrop - regExp.cpp

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
