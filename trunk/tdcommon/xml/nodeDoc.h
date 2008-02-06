#ifndef NODEDOC_H_
#define NODEDOC_H_

#include "xmlFile.h"

class nodeDoc {
public:
	nodeDoc(xmlFile *, string);
	nodeDoc(xmlFile *, string, nodeDoc);
	bool isValid();
	void next();
	virtual ~nodeDoc();
	string getAttributeValueByName(string n);
private:
	xmlNode *node;
	string name;
	xmlFile *xf;
};

#endif /*NODEDOC_H_*/
