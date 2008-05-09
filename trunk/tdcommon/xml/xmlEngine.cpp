/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "xmlEngine.h"

xmlEngine::xmlEngine() {
    init = NULL;
    query = NULL;
    gh = NULL;
    header = NULL;
    result = NULL;
    xml = new xmlFile();
}

xmlEngine::~xmlEngine() {
	if (init) delete init;
	if (query) delete query;
	if (header) delete header;
	if (result) delete result;
	delete xml;
}

bool xmlEngine::openEngine(string f) {
    string fullfilename;
    if (!selectFile::find(f,fullfilename)) return false;
    return xml->openFile(fullfilename);
}

void xmlEngine::setHttp(getHttp *g) {
    gh = g;
}

string xmlEngine::getName() {
	nodeDoc n(xml,"name");
	if (!n.isValid()) {
		cerr << "Can't find name node in " << xml->getFilename() << endl;
		return "";
	}
	return n.getNodeValue();
}

string xmlEngine::getCharset() {
	nodeDoc n(xml,"charset");
	if (!n.isValid()) {
		return "";
	}
	return n.getNodeValue();
}

bool xmlEngine::shouldUpdate() {
    nodeDoc n(xml,"updatable");
	if (n.isValid() && n.getNodeValue() == "no") {
		return false;
	}
	return true;
}


string xmlEngine::getUpdateUrl() {
	nodeDoc n(xml,"update");
	if (!n.isValid()) {
		return "";
	}
	return n.getNodeValue();
}

string xmlEngine::getSeparator() {
	nodeDoc n(xml,"separator");
    return n.getAttributeValueByName("value");
}

string xmlEngine::getInitUrl() {
	return getUrl(init);
}

string xmlEngine::getQueryUrl() {
	return getUrl(query);
}

string xmlEngine::getUrl(nodeDoc *nd) {
	nodeDoc n = *nd;
	if (!n.isValid()) {
		cerr << "Can't find node in " << xml->getFilename() << endl;
		return "";
	}
	
	n.findChildByName("url");
	if (!n.isValid()) {
		cerr << "Can't find init/url node in " << xml->getFilename() << endl;
		return "";
	}
	return n.getNodeValue();
}

methodParam xmlEngine::getInitInputFields(engineResults *res, regExp *reg, bool methodGet) {
	return getInputFields(res,reg,init,methodGet);
}

methodParam xmlEngine::getQueryInputFields(engineResults *res, regExp *reg, bool methodGet) {
	return getInputFields(res,reg,query,methodGet);
}

methodParam xmlEngine::getNextInputFields(engineResults *res, regExp *reg,bool methodGet,int r) {
	nodeDoc n(xml,"nextinput");
	return getInputFields(res,reg,&n,methodGet,r);
}

methodParam xmlEngine::getInputFields(engineResults *res, regExp *reg,nodeDoc *nd, bool methodGet, int results_per_pages) {
  string value;
 	methodParam param;
	string *output;
	string opt = "";
	nodeDoc n = *nd;
	if (!n.isValid()) {
		cerr << "Can't find node in " << xml->getFilename() << endl;
		return param;
	}
	n.findChildByName("inputfield");
	while(n.isValid()) {
	  if (methodGet || n.getAttributeValueByName("methodGET") == "yes") {
	    output = &param.get;
	  } else {
	    output = &param.post;
	  }
	    string fieldname = n.getAttributeValueByName("name");
	    if (*output != "" && fieldname !="") *output += "&";
			if (fieldname != "")
				*output += fieldname + "=";
			if ((value = n.getNodeValue()) == "") value = n.getAttributeValueByName("value");
			if (value.find("%u",0) != string::npos)
			  if ((opt = res->getOptions()[fieldname]) != "")
	  		  value = regExp::replaceAll(value,"%u",opt);
	  		else value = regExp::replaceAll(value,"%u",n.getAttributeValueByName("default"));
	  	string q;
			if ((q = generateQuery(res)) != "" && value.find("%q",0) != string::npos)
				value = regExp::replaceAll(value,"%q",q);
			if (reg != NULL && value.find("%s",0) != string::npos) {
				string num = n.getAttributeValueByName("num");
				if (num != "") {
					string nout = reg->getMatch(atoi(num.c_str()));
					if (n.getAttributeValueByName("escape") != "no") nout = gh->escape(nout);
					if (num != "") value = regExp::replaceAll(value,"%s",nout);
				} else {
					cerr << "Warning, %s used in inputfield without association to a num tag" << endl;
				}
			}
	    if (results_per_pages > -1 && value.find("%n",0) != string::npos) {
	        value = regExp::replaceAll(value,"%n",xml->itoa(results_per_pages));
	    }
	    if (results_per_pages > -1 && value.find("%m",0) != string::npos) {
	        value = regExp::replaceAll(value,"%m",xml->itoa(results_per_pages+1));
	    }
			*output += value;
			n.next();
	}
	return param;
}

string xmlEngine::generateQuery(engineResults *res) {
	string q = res->getQuery();
	if (getCharset() != "") q = gh->charsetConvert(q, HTTP_DEFAULT_CHARSET, getCharset());
  q = regExp::replaceAll(q, " ", getSeparator());
  q = gh->escape(q);
  return q;
}

string xmlEngine::getInitMethod() {
	return getMethod(init);
}

string xmlEngine::getQueryMethod() {
	return getMethod(query);
}

string xmlEngine::getMethod(nodeDoc *nd) {
	//xmlAttr *attr;
	nodeDoc n = *nd;
	if (!n.isValid()) {
		cerr << "Can't find init node in " << xml->getFilename() << endl;
		return "";
	}
	return n.getAttributeValueByName("method");
}

string xmlEngine::getResultRegExp() {
	nodeDoc n(xml,"result");
	
	if (!n.isValid()) {
		cerr << "Can't find result node in " << xml->getFilename() << endl;
		return "";
	}
	n.findChildByName("regexp");
	if (!n.isValid()) {
		cerr << "Can't find result/regexp node in " << xml->getFilename() << endl;
		return "";
	}
	return n.getNodeValue();
}

bool xmlEngine::hasNextInput() {
	nodeDoc n(xml, "nextinput");
    return n.isValid();
}

string xmlEngine::getNextInputRegExp() {
	nodeDoc n(xml, "nextinput");
	if (!n.isValid()) {
		//cerr << "Can't find nextinput node in " << filename << endl;
		return "";
	}
	n.findChildByName("regexp");
	if (!n.isValid()) {
		return "";
	}
	return n.getNodeValue();
}

string xmlEngine::getNextInputMethod() {
	nodeDoc n(xml, "nextinput");
	if (!n.isValid()) {
		cerr << "Can't find nextinput node in " << xml->getFilename() << endl;
		return "";
	}
    return n.getAttributeValueByName("method");
}

string xmlEngine::getNextInputUrl() {
	nodeDoc n(xml, "nextinput");
	if (!n.isValid()) {
		cerr << "Can't find nextinput node in " << xml->getFilename() << endl;
		return "";
	}
	n.findChildByName("url");
	if (!n.isValid()) {
		return "";
	}
	return n.getNodeValue();
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
	nodeDoc n = *init;
	n.findChildByName("regexp");
	return n.getNodeValue();
}

bool xmlEngine::getNextQuery() {
	if (query == NULL) query = new nodeDoc(xml,"query");
	else query->next();
	if (query == NULL) return false;
    return query->isValid();
}

bool xmlEngine::getNextInit() {
	if (init == NULL) init = new nodeDoc(xml,"init");
	else init->next();
	if (init == NULL) return false;
    return init->isValid();
}

bool xmlEngine::getNextResultField() {
	if (result == NULL) {
		nodeDoc res(xml,"result");
		result = new nodeDoc(xml,"field", res);
	} else result->next();
	if (result == NULL) return false;
    return result->isValid();
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
	if (!result) return "";
	nodeDoc n = *result;
    return n.getAttributeValueByName("name");
}

int xmlEngine::getResultFieldNumber() {
	if (!result) return 0;
	nodeDoc n = *result;
    string v = n.getAttributeValueByName("num");
    if (v != "") return atoi(v.c_str());
    return 0;
}

bool xmlEngine::getResultFieldComparable() {
	if (!result) return false;
	nodeDoc n = *result;
    return (n.getAttributeValueByName("comparable") == "yes");
}

bool xmlEngine::getResultFieldStripHTML() {
	if (!result) return false;
	nodeDoc n = *result;
    return (n.getAttributeValueByName("striphtml") == "yes");
}

bool xmlEngine::getResultFieldEscape() {
	if (!result) return false;
	nodeDoc n = *result;
    return (n.getAttributeValueByName("escape") == "yes");
}

string xmlEngine::getResultFieldValue() {
	if (!result) return "";
    return result->getNodeValue();
}

void xmlEngine::initResultField() {
	if (result) delete result;
    result = NULL;   
}

bool xmlEngine::getNextHeader() {
	if (header == NULL) header = new nodeDoc(xml,"header");
	else header->next();
	if (header == NULL) return false;
    return header->isValid();
}

string xmlEngine::getHeader() {
    return header->getNodeValue();
}

