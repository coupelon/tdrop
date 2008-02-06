/** Teardrop - openSave.cpp

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

#include "openSave.h"

bool openSave::xmlSave(string file, string query, list<string> engines, string limit, vector<row> r) {
    /* I don't wanted to use libxml to create such a simple output file, so i used ofstreams */
    getHttp gh;
    ofstream myfile;
    myfile.open(file.c_str(),ios::out|ios::trunc);
    if (myfile.is_open()) {
        myfile << "<teardrop>" << endl;
        //Engines
        myfile << "\t<engines>" << endl;
        for(list<string>::iterator lit = engines.begin(); lit != engines.end(); ++ lit) {
            myfile << "\t\t<name>" << gh.escape(*lit) << "</name>" << endl;
        }
        myfile << "\t</engines>" << endl;
        //Query
        myfile << "\t<query>" << gh.escape(query) << "</query>" << endl;
        //Limit
        myfile << "\t<limit>" << gh.escape(limit) << "</limit>" << endl;
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
            for(map<string, string, compstr>::iterator flds = lit->getFields().begin(); flds != lit->getFields().end(); ++flds) {
                myfile << "\t\t<fields name=\"" << gh.escape(flds->first) << "\">" << gh.escape(flds->second) << "</fields>" << endl;
            }
            myfile << "\t</row>" << endl;
        }
        myfile << "</teardrop>" << endl;
        return true;
    }
    return false;
}

bool openSave::xmlOpen(string file, string & query, list<string> & engines, string & limit, vector<row> & r) {
    getHttp gh;
    xmlFile input;
    if (input.openFile(file)) {
        xmlNode *cur = input.findChildByName(NULL,"engines");
        if(!cur) return false;
        cur = input.findChildByName(cur,"name");
        while(cur) {
            engines.push_back(gh.unescape(input.getNodeValue(cur)));
            cur = input.findNodeByName(cur->next,"name");
        }
        
        cur = input.findChildByName(NULL,"query");
        if (!cur) return false;
        query = gh.unescape(input.getNodeValue(cur));
        
        cur = input.findChildByName(NULL,"limit");
        if (!cur) return false;
        limit = gh.unescape(input.getNodeValue(cur));
        
        cur = input.findChildByName(NULL,"row");
        while(cur) {
            xmlNode *xmlRow = input.findChildByName(cur,"number");
            if(!xmlRow) return false;
            row rw(atoi(input.getNodeValue(xmlRow).c_str()));
            
            xmlRow = input.findChildByName(cur,"engines");
            if(!xmlRow) return false;
            xmlRow = input.findChildByName(xmlRow,"name");
            while(xmlRow) {
                rw.addEngine(gh.unescape(input.getNodeValue(xmlRow)));
                xmlRow = input.findNodeByName(xmlRow->next,"name");
            }
            xmlRow = input.findChildByName(cur,"fields");
            if(!xmlRow) return false;
            while(xmlRow) {
                rw.addField(gh.unescape(input.getNodeValue(xmlRow)), gh.unescape(input.getAttributeValueByName(xmlRow,"name")));
                xmlRow = input.findNodeByName(xmlRow->next,"fields");
            }
            r.push_back(rw);
            cur = input.findNodeByName(cur->next,"row");
        }
        return true;
    }
    return false;
}

bool openSave::htmlExport(string file, string query, list<string> engines, string limit, vector<row> r) {
    getHttp gh;
    ofstream myfile;
    myfile.open(file.c_str(),ios::out|ios::trunc);
    if (myfile.is_open()) {
        myfile << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=" << HTTP_DEFAULT_CHARSET << "\"><title>Teardrop search results</title></head><body>" << endl;
        myfile << "<h1>Query</h1>" << endl;
        myfile << "<ul>" << endl;
        //Engines
        myfile << "\t<li>Engines : ";
        for(list<string>::iterator lit = engines.begin(); lit != engines.end(); ++ lit) {
            myfile << *lit << " ";
        }
        myfile << "</li>" << endl;
        //Query
        myfile << "\t<li>Query : " << query << "</li>" << endl;
        //Limit
        myfile << "\t<li>Results per engines : " << limit << "</li>" << endl;
        //Rows
        myfile << "</ul>"<< endl;
        myfile << "<h1>Results</h1>" << endl;
        myfile << "<table>" << endl;
        for(vector<row>::iterator lit = r.begin(); lit != r.end(); ++ lit) {
            myfile << "\t<tr>" << endl;
            myfile << "\t\t<td>" << lit->getNum() << "</td><td><ul>" << endl;
            myfile << "\t\t<li>Engines : ";
            list<string> lrEngines = lit->getEngine();
            for(list<string>::iterator englst = lrEngines.begin(); englst != lrEngines.end(); ++ englst) {
                myfile << *englst << " ";
            }
            myfile << "\t\t</li>" << endl;
            for(map<string, string, compstr>::iterator flds = lit->getFields().begin(); flds != lit->getFields().end(); ++flds) {
                if (flds->first != "url")
                    myfile << "\t\t<li>" << flds->first << " : " << flds->second << "</li>" << endl;
                else
                    myfile << "\t\t<li>" << flds->first << " : <a href=\"" << flds->second << "\">" << flds->second << "</a></li>" << endl;
                
            }
            myfile << "\t</ul></td></tr>" << endl;
        }
        myfile << "</table></body></html>" << endl;
        return true;
    }
    return false;
}

bool openSave::csvExport(string file, vector<row> r) {
    /* In fact I'll split the format with semicolon, not comma,
    to remain compliant with MS Excel */
    set<string> fields;
    ofstream myfile;
    myfile.open(file.c_str(),ios::out|ios::trunc);
    if (myfile.is_open()) {
        /* Creates the list of all fields */
        for(vector<row>::iterator lit = r.begin(); lit != r.end(); ++ lit) {
            for(map<string, string, compstr>::iterator flds = lit->getFields().begin(); flds != lit->getFields().end(); ++flds) {
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
        for(vector<row>::iterator lit = r.begin(); lit != r.end(); ++ lit) {
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
        return true;
    }
    return false;
}
