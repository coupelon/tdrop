#include "nodeDoc.h"

nodeDoc::nodeDoc(xmlFile *xf, string nd) {
	name = nd;
	this->xf = xf;
	node = xf->findChildByName(NULL,nd);
}

nodeDoc::nodeDoc(xmlFile *xf, string nd, nodeDoc parent) {
	name = nd;
	this->xf = xf;
	node = xf->findChildByName(parent.node,nd);
}

bool nodeDoc::isValid() {
	return (node != NULL);
}

void nodeDoc::next() {
	node = xf->findNodeByName(node->next,name);
}

string nodeDoc::getAttributeValueByName(string n) {
	return xf->getAttributeValueByName(node,n);
}

nodeDoc::~nodeDoc() {
}
