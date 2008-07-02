/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "openSave.h"

bool openSave::xmlSave(string file, string query, map<string, int> engines, unsigned long limit, vector<row> r) {
    /* I don't wanted to use libxml to create such a simple output file, so i used ofstreams */
    getHttp gh;
    ofstream myfile;
    myfile.open(file.c_str(),ios::out|ios::trunc);
    if (myfile.is_open()) {
        myfile << "<teardrop>" << endl;
        //Engines
        myfile << "\t<engines>" << endl;
        for(map<string, int>::iterator lit = engines.begin(); lit != engines.end(); ++ lit) {
            myfile << "\t\t<name results=\"" << lit->second << "\">" << gh.escape(lit->first) << "</name>" << endl;
        }
        myfile << "\t</engines>" << endl;
        //Query
        myfile << "\t<query>" << gh.escape(query) << "</query>" << endl;
        //Limit
        myfile << "\t<limit>" << limit << "</limit>" << endl;
        //Rows
        for(vector<row>::iterator lit = r.begin(); lit != r.end(); ++ lit) {
            myfile << "\t<row>" << endl;
            myfile << "\t\t<number>" << lit->getNum() << "</number>" << endl;
            myfile << "\t\t<engines>" << endl;
            list<string> lrEngines = lit->getEngine();
            for(list<string>::iterator englst = lrEngines.begin(); englst != lrEngines.end(); ++ englst) {
                myfile << "\t\t\t<name>" << gh.escape(*englst) << "</name>" << endl;
            }
            myfile << "\t\t</engines>" << endl;
            for(map<string, string, compstr>::const_iterator flds = lit->getFields().begin(); flds != lit->getFields().end(); ++flds) {
                myfile << "\t\t<fields name=\"" << gh.escape(flds->first) << "\">" << gh.escape(flds->second) << "</fields>" << endl;
            }
            myfile << "\t</row>" << endl;
        }
        myfile << "</teardrop>" << endl;
        return true;
    }
    return false;
}

bool openSave::xmlSave(string file, metaRank *mr) {
	return xmlSave(file,
					mr->getEngineResults().getQuery(),
					mr->getEngineResults().getEngines(),
					mr->getEngineResults().getLimit(),
					mr->getEngineResults().getResults());
	return false;
}

bool openSave::xmlOpen(const string & file, metaRank **mr, tdParam *tdp) {
    xmlFile input;
    if (input.openFile(file)) {
    vector<row> r;
		getHttp gh;
  	engineResults *er = new engineResults();
	
  	nodeDoc *cur = new nodeDoc(&input,"engines");
    if(!cur->isValid()) return false;
    cur->findChildByName("name");
    while(cur->isValid()) {
        er->addEngine(gh.unescape(cur->getNodeValue()),atoi(cur->getAttributeValueByName("results").c_str()));
        cur->next();
    }
    
    cur->findChildByName("query",NULL);
    if (!cur->isValid()) return false;
    er->setQuery(gh.unescape(cur->getNodeValue()));
    
    cur->findChildByName("limit",NULL);
    if (!cur->isValid()) return false;
		er->setLimit(atol(gh.unescape(cur->getNodeValue()).c_str()));
        
    cur->findChildByName("row",NULL);
    while(cur->isValid()) {
        nodeDoc *xmlRow = new nodeDoc(&input,"number",*cur);
        if(!xmlRow->isValid()) return false;
        row rw(atoi(xmlRow->getNodeValue().c_str()));
        
        xmlRow->findChildByName("engines",cur);
        if(!xmlRow->isValid()) return false;
        xmlRow->findChildByName("name");
        while(xmlRow->isValid()) {
            rw.addEngine(gh.unescape(xmlRow->getNodeValue()));
            xmlRow->next();
        }
        xmlRow->findChildByName("fields",cur);
        if(!xmlRow->isValid()) return false;
        while(xmlRow->isValid()) {
            rw.addField(gh.unescape(xmlRow->getNodeValue()), gh.unescape(xmlRow->getAttributeValueByName("name")));
            xmlRow->next();
        }
        r.push_back(rw);
        cur->next();
    }
    er->setResults(r);
		*mr = new metaRank(er,tdp);
		(*mr)->setFinished();
		return true;
    }
    return false;
}

bool openSave::xmlOpenHeader(const string & file, engineResults *er) {
  xmlFile input;
  if (input.openFile(file)) {
  	vector<row> r;
	  getHttp gh;
	
  	nodeDoc *cur = new nodeDoc(&input,"engines");
      if(!cur->isValid()) return false;
      cur->findChildByName("name");
      while(cur->isValid()) {
          er->addEngine(gh.unescape(cur->getNodeValue()),atoi(cur->getAttributeValueByName("results").c_str()));
          cur->next();
      }
      
      cur->findChildByName("query",NULL);
      if (!cur->isValid()) return false;
      er->setQuery(gh.unescape(cur->getNodeValue()));
      
      cur->findChildByName("limit",NULL);
      if (!cur->isValid()) return false;
	    er->setLimit(atol(gh.unescape(cur->getNodeValue()).c_str()));
      
      /* cur->findChildByName("row",NULL);
      int count = 0;
      while(cur->isValid()) {
          count++;
          cur->next();
      } */
	  return true;
  }
  return false;
}

void openSave::htmlExport(stringstream & myfile, metaRank *mr) {
    myfile << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=" << HTTP_DEFAULT_CHARSET << "\"><title>Teardrop search results</title></head><body>" << endl;
    myfile << "<div class=\"main_title\">Query</div>" << endl;
    myfile << "<div class=\"info\">" << endl;
    //Engines
    myfile << "\t<div class=\"engines\">Engines : ";
    for(map<string, int>::iterator lit = mr->getEngineResults().getEngines().begin(); lit != mr->getEngineResults().getEngines().end(); ++ lit) {
        myfile << lit->first << "(" << lit->second << ") ";
    }
    myfile << "</div>" << endl;
    //Query
    myfile << "\t<div class=\"query\">Query : " << mr->getEngineResults().getQuery() << "</div>" << endl;
    //Limit
    myfile << "\t<div class=\"limit\">Results per engines : " << mr->getEngineResults().getLimit() << "</div>" << endl;
    //Rows
    myfile << "</div>"<< endl;
    myfile << "<div class=\"title\">Results</div>" << endl;
    myfile << "<div class=\"table\">" << endl;
    for(vector<row>::iterator lit = mr->getEngineResults().getResults().begin(); lit != mr->getEngineResults().getResults().end(); ++ lit) {
        myfile << "\t<div class=\"line\">" << endl;
        myfile << "\t\t<div class=\"num\">" << lit->getNum() << "</div><div class=\"engines\">" << endl;
        list<string> lrEngines = lit->getEngine();
        for(list<string>::iterator englst = lrEngines.begin(); englst != lrEngines.end(); ++ englst) {
            myfile << *englst << " ";
        }
        myfile << "\t\t</div>" << endl;
        for(map<string, string, compstr>::const_iterator flds = lit->getFields().begin(); flds != lit->getFields().end(); ++flds) {
//                if (flds->first != "url")
//                    myfile << "\t\t<li>" << flds->first << " : " << flds->second << "</li>" << endl;
//                else
//                    myfile << "\t\t<li>" << flds->first << " : <a href=\"" << flds->second << "\">" << flds->second << "</a></li>" << endl;
				  
				  if (flds->first != "url")
				  	myfile << "\t\t<span class=\"" << flds->first << "\">" << flds->second << "</span>" << endl;
				  else
				    myfile << "\t\t<a class=\"" << flds->first << "\" href=\"" << flds->second << "\">" << flds->second << "</a>" << endl;
            
        }
        myfile << "\t</div>" << endl;
    }
    myfile << "</div></body></html>" << endl;
}

bool openSave::htmlExport(string & file, metaRank *mr) {
	getHttp gh;
    ofstream myfile;
    myfile.open(file.c_str(),ios::out|ios::trunc);
    if (myfile.is_open()) {
    	stringstream ss;
		htmlExport(ss, mr);
		myfile << ss;
		return true;
    }
    return false;
}

void openSave::csvExport(stringstream & myfile, const vector<row> & r) {
	/* In fact I'll split the format with semicolon, not comma,
    to remain compliant with MS Excel */
    set<string> fields;
    /* Creates the list of all fields */
    for(vector<row>::const_iterator lit = r.begin(); lit != r.end(); ++ lit) {
        for(map<string, string, compstr>::const_iterator flds = lit->getFields().begin(); flds != lit->getFields().end(); ++flds) {
            fields.insert(flds->first);
        }
    }
    // Outputs the headers
    myfile << "\"engines\"";
    for(set<string>::iterator sit = fields.begin(); sit != fields.end(); ++ sit) {
        myfile << ";\"" << *sit << "\"";
    }
    myfile << endl;
    // Outputs the values
    for(vector<row>::const_iterator lit = r.begin(); lit != r.end(); ++ lit) {
        myfile << "\"";
        list<string> lrEngines = lit->getEngine();
        for(list<string>::iterator englst = lrEngines.begin(); englst != lrEngines.end(); ++ englst) {
            if (englst != lrEngines.begin()) myfile << ";";
            myfile << *englst;
        }
        myfile << "\"";
        for(set<string>::iterator sit = fields.begin(); sit != fields.end(); ++ sit) {
            myfile << ";\"" << regExp::replaceAll(lit->getField(*sit),"\"","\"\"") << "\"";
        }
        myfile << endl;
    }
}

bool openSave::csvExport(string file, const vector<row> & r) {
    ofstream myfile;
    myfile.open(file.c_str(),ios::out|ios::trunc);
    if (myfile.is_open()) {
    	stringstream ss;
		csvExport(ss, r);
		myfile << ss;
		return true;
    }
    return false;
}


string openSave::csvExport(metaRank *mr) {
	stringstream ss;
	csvExport(ss, mr->getResults());
	return ss.str();
}
