/** Teardrop - xmlEngine.cpp

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

#include "xmlEngine.h"

xmlEngine::xmlEngine() : xmlFile() {
    init = NULL;
    query = NULL;
    gh = NULL;
    header = NULL;
}

xmlEngine::~xmlEngine() {
}

bool xmlEngine::openEngine(string f) {
    string path;
    if (!selectFile::find(f,".xml",path)) return false;
    return xmlFile::openFile(path + f + ".xml");
}

void xmlEngine::setHttp(getHttp *g) {
    gh = g;
}

string xmlEngine::getName() {
	xmlNode *n;
	n = findChildByName(root_element,"name");
	if (n == NULL) {
		cerr << "Can't find name node in " << filename << endl;
		return "";
	}
	return getNodeValue(n);
}

string xmlEngine::getCharset() {
	xmlNode *n;
	n = findChildByName(root_element,"charset");
	if (n == NULL) {
		return "";
	}
	return getNodeValue(n);
}

bool xmlEngine::shouldUpdate() {
    xmlNode *n;
	n = findChildByName(root_element,"updatable");
	if (n != NULL && getNodeValue(n) == "no") {
		return false;
	}
	return true;
}


string xmlEngine::getUpdateUrl() {
    xmlNode *n;
	n = findChildByName(root_element,"update");
	if (n == NULL) {
		return "";
	}
	return getNodeValue(n);
}

string xmlEngine::getSeparator() {
    return getAttributeValueByName(findChildByName(root_element,"separator"), "value");
}

string xmlEngine::getInitUrl() {
	return getUrl(init);
}

string xmlEngine::getQueryUrl() {
	return getUrl(query);
}

string xmlEngine::getUrl(xmlNode *nd) {
	xmlNode *n;
	n = nd;
	if (n == NULL) {
		cerr << "Can't find node in " << filename << endl;
		return "";
	}
	n = findChildByName(n, "url");
	if (n == NULL) {
		cerr << "Can't find init/url node in " << filename << endl;
		return "";
	}
	return getNodeValue(n);
}

methodParam xmlEngine::getInitInputFields(engineResults *res, regExp *reg, bool methodGet) {
	return getInputFields(res,reg,init,methodGet);
}

methodParam xmlEngine::getQueryInputFields(engineResults *res, regExp *reg, bool methodGet) {
	return getInputFields(res,reg,query,methodGet);
}

methodParam xmlEngine::getNextInputFields(engineResults *res, regExp *reg,bool methodGet,int r) {
	return getInputFields(res,reg,findChildByName(root_element,"nextinput"),methodGet,r);
}

methodParam xmlEngine::getInputFields(engineResults *res, regExp *reg,xmlNode *nd, bool methodGet, int r) {
	xmlNode *n;
  string value;
 	methodParam param;
	string *output;
	string opt = "";
	n = nd;
	if (n == NULL) {
		cerr << "Can't find node in " << filename << endl;
		return param;
	}
	n = findNodeByName(n->children,"inputfield");
	while(n) {
	  if (methodGet || getAttributeValueByName(n,"methodGET") == "yes") {
	    output = &param.get;
	  } else {
	    output = &param.post;
	  }
    string fieldname = getAttributeValueByName(n,"name");
    if (*output != "" && fieldname !="") *output += "&";
		if (fieldname != "")
			*output += fieldname + "=";
		if ((value = getNodeValue(n)) == "") value = getAttributeValueByName(n,"value");
		if (value.find("%u",0) != string::npos)
		  if ((opt = res->getOptions()[fieldname]) != "")
  		  value = regExp::replaceAll(value,"%u",opt);
  		else value = regExp::replaceAll(value,"%u",getAttributeValueByName(n,"default"));
		if (res->getQuery() != "" && value.find("%q",0) != string::npos)
			value = regExp::replaceAll(value,"%q",res->getQuery());
		if (reg != NULL && value.find("%s",0) != string::npos) {
			string num = getAttributeValueByName(n,"num");
			if (num != "") {
				string nout = reg->getMatch(atoi(num.c_str()));
				if (getAttributeValueByName(n,"escape") != "no") nout = gh->escape(nout);
				if (num != "") value = regExp::replaceAll(value,"%s",nout);
			} else {
				cerr << "Warning, %s used in inputfield without association to a num tag" << endl;
			}
		}
    if (r > -1 && value.find("%n",0) != string::npos) {
        value = regExp::replaceAll(value,"%n",itoa(r));
    }
    if (r > -1 && value.find("%m",0) != string::npos) {
        value = regExp::replaceAll(value,"%m",itoa(r+1));
    }
		*output += value;
		n = findNodeByName(n->next,"inputfield");
	}
	return param;
}

string xmlEngine::getInitMethod() {
	return getMethod(init);
}

string xmlEngine::getQueryMethod() {
	return getMethod(query);
}

string xmlEngine::getMethod(xmlNode *nd) {
	xmlNode *n;
	xmlAttr *attr;
	n = nd;
	if (n == NULL) {
		cerr << "Can't find init node in " << filename << endl;
		return "";
	}
	if ((attr = getAttributeByName(n,"method")))
		return (char *) attr->children->content;
	return "";
}

string xmlEngine::getResultRegExp() {
	xmlNode *n;
	n = findChildByName(root_element,"result");
	if (n == NULL) {
		cerr << "Can't find result node in " << filename << endl;
		return "";
	}
	n = findChildByName(n, "regexp");
	if (n == NULL) {
		cerr << "Can't find result/regexp node in " << filename << endl;
		return "";
	}
	return getNodeValue(n);
}

bool xmlEngine::hasNextInput() {
    return (findChildByName(root_element,"nextinput") != NULL);
}

string xmlEngine::getNextInputRegExp() {
	xmlNode *n;
	n = findChildByName(root_element,"nextinput");
	if (n == NULL) {
		//cerr << "Can't find nextinput node in " << filename << endl;
		return "";
	}
	n = findChildByName(n, "regexp");
	if (n == NULL) {
		return "";
	}
	return getNodeValue(n);
}

string xmlEngine::getNextInputMethod() {
	xmlNode *n;
   
	n = findChildByName(root_element,"nextinput");
	if (n == NULL) {
		cerr << "Can't find nextinput node in " << filename << endl;
		return "";
	}
    
    return getAttributeValueByName(n,"method");
}

string xmlEngine::getNextInputUrl() {
	xmlNode *n;
	n = findChildByName(root_element,"nextinput");
	if (n == NULL) {
		cerr << "Can't find nextinput node in " << filename << endl;
		return "";
	}
	n = findChildByName(n, "url");
	if (n == NULL) {
		return "";
	}
	return getNodeValue(n);
}

address xmlEngine::getNextInputAddress(engineResults *res, regExp *reg, int r) {
	address a;
	a.url = getNextInputUrl();
	if (getNextInputMethod() == "GET") a.setGET();
	else a.setPOST();
	a.param = getNextInputFields(res, reg, a.isGET(), r);
	return a;
}

address xmlEngine::getInitAddress(engineResults *res, regExp *r) {
	address a;
	a.url = getInitUrl();
	if (getInitMethod() == "GET") a.setGET();
	else a.setPOST();
	a.param = getInitInputFields(res, r,a.isGET());
	return a;
}

string xmlEngine::getInitRegExp() {
	if (!init) return "";
	xmlNode *n = init;
	n = findChildByName(n, "regexp");
	if (!n) {
		return "";
	}
	return (char *) n->children->content;
}

bool xmlEngine::getNextQuery() {
    return getNext(&query,"query");
}

bool xmlEngine::getNextInit() {
    return getNext(&init,"init");
}

bool xmlEngine::getNextResultField() {
    return getNext(&result,"field", findChildByName(root_element,"result"));
}

bool xmlEngine::getNext(xmlNode **t, string name, xmlNode *base_element) {
    xmlNode *n;
    if (!base_element) base_element = root_element;
    if (!*t) n = findChildByName(base_element,name);
    else n = findNodeByName((*t)->next,name);
    if (n == NULL) {
        return false;
    }
    *t = n;
    return true;
}

address xmlEngine::getQueryAddress(engineResults *res, regExp *r) {
	address a;
	a.url = getQueryUrl();
	if (getQueryMethod() == "GET") a.setGET();
	else a.setPOST();
	a.param = getQueryInputFields(res, r,a.isGET());
	return a;
}

string xmlEngine::getResultFieldName() {
    return getAttributeValueByName(result,"name");
}

int xmlEngine::getResultFieldNumber() {
    string v = getAttributeValueByName(result,"num");
    if (v != "") return atoi(v.c_str());
    return 0;
}

bool xmlEngine::getResultFieldComparable() {
    return (getAttributeValueByName(result,"comparable") == "yes");
}

bool xmlEngine::getResultFieldStripHTML() {
    return (getAttributeValueByName(result,"striphtml") == "yes");
}

bool xmlEngine::getResultFieldEscape() {
    return (getAttributeValueByName(result,"escape") == "yes");
}

string xmlEngine::getResultFieldValue() {
    return getNodeValue(result);
}

void xmlEngine::initResultField() {
    result = NULL;   
}

bool xmlEngine::getNextHeader() {
    return getNext(&header,"header");
}

string xmlEngine::getHeader() {
    return getNodeValue(header);
}

