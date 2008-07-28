/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "pageParser.h"

pageParser::pageParser(const string & e, rowContainer *ler, threadPool<pageParser> *mut, tdParam *t) {
	engname = e;
	eng = NULL;
	query = ler->getQuery();
	limit = ler->getLimit();
	global_results = ler;
	results_lock = mut;
  tdp = t;
  //setAbort(false);
}

pageParser::pageParser(const pageParser & pp) {
	engname = pp.engname;
	eng = NULL;
	query = pp.query;
	limit = pp.limit;
	pages = pp.pages;
	visited_pages = pp.visited_pages;
	results = pp.results;
	global_results = pp.global_results;
	results_lock = pp.results_lock;
  tdp = pp.tdp;
  //abort = pp.abort;
}

pageParser::~pageParser() {
	if (eng) delete eng;
}

bool pageParser::initParse() {
	string page;
	regExp *r = NULL;
	eng = new xmlEngine();
  if (!eng->openEngine(engname)) return false;
  eng->setHttp(&gh);
	gh.setAcceptCharset(eng->getCharset());
  gh.setParam(tdp);
  while(eng->getNextHeader()) {
      gh.addHttpHeader(eng->getHeader());
  }
	while(eng->getNextInit()) {
		address ad = eng->getInitAddress(global_results,r);
		page = gh.getPage(ad);
		string reg;
		if ((reg = eng->getInitRegExp()) != "") {
			if (r) delete r;
			r = new regExp(reg, page);
		}
	}

	while(eng->getNextQuery()) {
		pages.push_front(eng->getQueryAddress(global_results,r));
	}
	if (r) delete r;
    return true;
}
	
void pageParser::doParse() {
    regExp *r;
    regExp *n;
    if(eng->getResultRegExp() != "") r = new regExp(eng->getResultRegExp());
    else r = NULL;
    bool hasNext = eng->hasNextInput();
    if(hasNext) {
        string nire = eng->getNextInputRegExp();
        if (nire != "") n = new regExp(nire);
        else n = NULL;
    } else n = NULL;
	while(results.size() < limit) {
		//pthread_testcancel();
		if (getAbort()) break;
		address ad = getNextAddress();
		if (ad.isEmpty()) { 
			LOG4CXX_INFO(tdParam::logger, "Can't find enough results for " + engname);
			break;
		}
		string page = gh.getPage(ad,eng->getCharset());
    //pthread_testcancel();
    if (getAbort()) break;
    if (tdp->getEcho()) cout << ">" << ad << endl;
		if (page == "") { 
			LOG4CXX_INFO(tdParam::logger, "Invalid url : " + ad.getFullUrl());
			break;
		}

    if (tdp->getEcho()) cout << "<<<<<<" << endl << page << ">>>>>>" << endl;
    
    if (r) {
        //Loop until every or enough link are parsed
        for(r->newPage(page); results.size() < limit && !r->endOfMatch(); r->next()) {
            row rw = generateRow(*r);
            if (find(results.begin(),results.end(), rw) == results.end())
                results.push_back(rw);
        }
    }
    
    if (n) {      
      for(n->newPage(page);
          !n->endOfMatch(); n->next()) {
	          pages.push_back(eng->getNextInputAddress(global_results,n,results.size()));
      }
    } else if (hasNext) pages.push_back(eng->getNextInputAddress(global_results,NULL,results.size()));
	}
  if (n) delete n;
  if (r) delete r;
}

void pageParser::closeParse(bool validEngine) {
	if (getAbort()) results.clear();
	threadPool<pageParser>::lock(results_lock);
	if (validEngine)
		global_results->addRankedResults(results,engname);
	else
		global_results->addRankedResults(results,engname);
	threadPool<pageParser>::unlock(results_lock);
}

void pageParser::startParsing() {
  gh.setAbort(global_results->getAbort());
  try {
	  if (initParse()) {
	    doParse();
	    closeParse();
	  } else closeParse(false);
  } catch (exception& e) {
      cerr << ">An exception was caught from Teardrop : " << e.what() << endl;
      closeParse(false);
  }
}

vector<row> & pageParser::getResults() {
	return results;
}

row pageParser::generateRow(regExp & r) {
    row l(results.size(),eng->getName());
    eng->initResultField();
    while(eng->getNextResultField()) {
        int number = eng->getResultFieldNumber();
        string value = eng->getResultFieldValue();
        string name = eng->getResultFieldName();
        if(number > 0) {
            string answer = r.getMatch(number);
            if (answer == "") continue;
            if (value != "") value = regExp::replaceAll(value,"%s", answer);
            else value = answer;
        }
        value = gh.moreUnescape(value);
        if (eng->getResultFieldComparable()) l.addComparable(name);
        if (eng->getResultFieldStripHTML()) {
            value = regExp::replaceAll(value,"&lt;","<");
            value = regExp::replaceAll(value,"&gt;",">");
            value = regExp::replaceAll(value,"&amp;","&");
            value = regExp::replaceAll(value,"&nbsp;"," ");
            value = regExp::replaceAll(value,"&quot;","\"");
            value = regExp::replaceAll(value,"<[^>]*>","");
        }
        //Removes every control characters
        value = regExp::replaceAll(value,"[[:cntrl:]]"," ");
        if (eng->getResultFieldEscape()) value = gh.escape(value);
        l.addField(value, name);
    }
	return l;
}

address pageParser::getNextAddress() {
	address ad;
	do {
	if (pages.empty()) {
		address nv;
		return nv;
	}
	ad = pages.front();
	pages.pop_front();
	} while (visited_pages.find(ad) != visited_pages.end());
	visited_pages.insert(ad);
	return ad;
}
