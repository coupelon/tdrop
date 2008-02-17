#ifndef NODEDOC_H_
#define NODEDOC_H_

#include "xmlFile.h"

class nodeDoc {
public:
	nodeDoc(xmlFile *, string);
	nodeDoc(xmlFile *, string, nodeDoc);
	nodeDoc(const nodeDoc &);
	bool isValid();
	void next();
	virtual ~nodeDoc();
	
	/** Returns the value of the given node */
  	string getNodeValue();

  	/** Returns the value of the given node's attribute name.
  	Returns "" if none found. */
  	string getAttributeValueByName(string);
  	
  	/** Sets the content of the given node. Create it if needed */
  	void setNodeContent(string);
  	
  	/** Jump to the child names */
  	void findChildByName(string);
  	void findChildByName(string, nodeDoc *);
  	
  	/** Jump to the first child */
  	void findFirstChild();
  	
private:
	/** This method tries to find the given attribute, by its name,
  	in the given xmlNode */
  	xmlAttr *getAttributeByName(string);
  	
  	/** This method tries to find the given tag, by its name,
	in the children of the given xmlNode */
	xmlNode *findChildByName(xmlNode *,string);
	
	/** This method tries to find the given tag, by its name,
    before the given xmlNode */
    xmlNode *findPreviousNodeByName(xmlNode *,string);
  	
	xmlNode *node;
	string name;
	xmlFile *xf;
};

#endif /*NODEDOC_H_*/
