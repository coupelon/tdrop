/** Teardrop - xmlFile.cpp

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

#include "xmlFile.h"

xmlFile::xmlFile() {
    filename = "";
    doc = NULL;
    root_element = NULL;
}

xmlFile::~xmlFile() {
    if (doc)
        xmlFreeDoc(doc);
    xmlCleanupParser();
}

bool xmlFile::openFile(string f) {
    filename = f;
    if (doc) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
    }
    doc = xmlReadFile(filename.c_str(), NULL, 0);
    if (doc == NULL) {
        return false;
    }
    root_element = xmlDocGetRootElement(doc);
    return true;   
}

void xmlFile::close() {
    if (doc)
        xmlFreeDoc(doc);
}

xmlNode *xmlFile::findChildByName(xmlNode *p, string c) {
    if (p == NULL) p = root_element;
    xmlNode *child = p->children;
    while(child) {
        if( child->type == XML_ELEMENT_NODE ) {
            string name = (char *) child->name;
            if (name == c) return child;
        }
        child = child->next;
    }
    return child;
}

xmlNode *xmlFile::findNodeByName(xmlNode *p, string c) {
    if (p == NULL) p = root_element;
    xmlNode *child = p;
    while(child) {
        if( child->type == XML_ELEMENT_NODE ) {
            string name = (char *) child->name;
            if (name == c) return child;
        }
        child = child->next;
    }
    return NULL;
}

xmlNode *xmlFile::findPreviousNodeByName(xmlNode *p, string c) {
    if (p == NULL) return NULL;
    xmlNode *child = p;
    while(child) {
        if( child->type == XML_ELEMENT_NODE ) {
            string name = (char *) child->name;
            if (name == c) return child;
        }
        child = child->prev;
    }
    return child;
}

xmlAttr *xmlFile::getAttributeByName(xmlNode *p,string c) {
    if (p == NULL) p = root_element;
    xmlAttr *attribute = p->properties;
    while( attribute ){
        string name = (char *) attribute->name;
        if (name == c) return attribute;
        attribute = attribute->next;
    }
    return attribute;
}

string xmlFile::getAttributeValueByName(xmlNode *n, string name) {
    if (n == NULL) n = root_element;
    xmlAttr *attr;
    if (n) {   
        if ((attr = getAttributeByName(n,name))) {
            if (attr->children && attr->children->content)
                return (char *) attr->children->content;
        }
    }
    return "";
}

string xmlFile::getNodeValue(xmlNode *n) {
    //if (n == NULL) n = root_element;
    if (n && n->children && n->children->content)
        return (char *) n->children->content;
    return "";
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

void xmlFile::setNodeContent(xmlNode *p, string s, string c) {
    if (!p) p = root_element;
    xmlNode *n = findChildByName(p,s.c_str());
    if (n) {
        xmlNodeSetContent(n,(const xmlChar*) c.c_str());
    } else {
        xmlNewChild(p,NULL,(const xmlChar*) s.c_str(),(const xmlChar*) c.c_str());
    }
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

