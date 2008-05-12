/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "nodeDoc.h"

nodeDoc::nodeDoc(xmlFile *xf) {
	name = "";
	this->xf = xf;
	node = xf->getRootElement();
}

nodeDoc::nodeDoc(xmlFile *xf, string nd) {
	name = nd;
	this->xf = xf;
	node = findChildByName(NULL,nd);
}

nodeDoc::nodeDoc(xmlFile *xf, string nd, nodeDoc parent) {
	name = nd;
	this->xf = xf;
	node = findChildByName(parent.node,nd);
}

nodeDoc::nodeDoc(const nodeDoc & n) {
	name = n.name;
	xf = n.xf;
	node = n.node;
}

nodeDoc::~nodeDoc() {
}

bool nodeDoc::isValid() {
	return (node != NULL);
}

void nodeDoc::next() {
	if (xf == NULL) return;
	xmlNode *child;
	if (node == NULL) {
		node = xf->getRootElement();
    	child = node;
	} else {
		child = node->next;
	}
    while(child) {
        if( child->type == XML_ELEMENT_NODE ) {
            string n = (char *) child->name;
            if (n == name) {
            	node = child;
 				return;
            }
        }
        child = child->next;
    }
    node = NULL;
    return;
}

string nodeDoc::getAttributeValueByName(string name) {
    if (node == NULL) node = xf->getRootElement();
    xmlAttr *attr;
    if (node) {   
        if ((attr = getAttributeByName(name))) {
            if (attr->children && attr->children->content)
                return (char *) attr->children->content;
        }
    }
    return "";
}

xmlAttr *nodeDoc::getAttributeByName(string c) {
    if (node == NULL) node = xf->getRootElement();
    xmlAttr *attribute = node->properties;
    while( attribute ){
        string name = (char *) attribute->name;
        if (name == c) return attribute;
        attribute = attribute->next;
    }
    return attribute;
}

string nodeDoc::getNodeValue() {
    if (node && node->children && node->children->content)
        return (char *) node->children->content;
    return "";
}

xmlNode *nodeDoc::findChildByName(xmlNode *p, string c) {
    if (p == NULL) p = xf->getRootElement();
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

xmlNode *nodeDoc::findPreviousNodeByName(xmlNode *p, string c) {
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

void nodeDoc::setChildContent(string name, string c) {
    if (name == "") return;
    if (!node) node = xf->getRootElement();
    xmlNode *child = findChildByName(node,name);
    if (child) {
        xmlNodeSetContent(child,(const xmlChar*) c.c_str());
    } else {
        xmlNewChild(node,NULL,(const xmlChar*) name.c_str(),(const xmlChar*) c.c_str());
        
    }
}

void nodeDoc::findChildByName(string nd) {
	name = nd;
	node = findChildByName(node,nd);
}

void nodeDoc::findChildByName(string nd, nodeDoc *n) {
	name = nd;
	node = findChildByName(((n==NULL)?NULL:n->node),nd);
}

void nodeDoc::findFirstChild() {
	if (node == NULL) return;
	node = node->children;
}
