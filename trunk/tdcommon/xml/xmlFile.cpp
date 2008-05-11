/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "xmlFile.h"

xmlFile::xmlFile() {
    filename = "";
    doc = NULL;
    root_element = NULL;
    rc = NULL;
}

xmlFile::~xmlFile() {
    if (doc)
        xmlFreeDoc(doc);
    if (rc)
        delete rc;
    xmlCleanupParser();
}

bool xmlFile::openFile(string f) {
    filename = f;
    if (doc) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        if (rc) {
        	delete rc;
        	rc = NULL;
        }
    }
    doc = xmlReadFile(filename.c_str(), NULL, 0);
    if (doc == NULL) {
        return false;
    }
    root_element = xmlDocGetRootElement(doc);
    return true;   
}

bool xmlFile::openUrl(string f, tdParam *tdp) {
    filename = f;
    getHttp gh;
    gh.setParam(tdp);
    address ad;
    ad.url = f;
    if (doc) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        if (rc) {
        	delete rc;
        	rc = NULL;
        }
    }
    rc = new rawContainer();
    
    if (!gh.getRawData(ad,rc)) return false;
    doc = xmlReadMemory(rc->getContent(), rc->getLength(), f.c_str(), NULL, 0);
    if (doc == NULL) {
        return false;
    }
    root_element = xmlDocGetRootElement(doc);
    return true;   
}

void xmlFile::close() {
    if (doc)
      xmlFreeDoc(doc);
    if (rc) {
    	delete rc;
    	rc = NULL;
    }
}



bool xmlFile::saveDocTo(string & file) {
    FILE *f = fopen(file.c_str(),"wb");
    if (f) {
        xmlOutputBufferPtr output = xmlOutputBufferCreateFile(f, NULL);
        if (output) {
            xmlSaveFileTo(output,doc,NULL);
            return true;
        }
        fclose(f);
    }
    return false;
}

string xmlFile::itoa(int i) {
    std::ostringstream ostr;
    ostr << i;
    return ostr.str();
}

string xmlFile::ltoa(long i) {
    std::ostringstream ostr;
    ostr << i;
    return ostr.str();
}

string & xmlFile::getFilename() {
	return filename;
}

xmlNode *xmlFile::getRootElement() {
	return root_element;
}
