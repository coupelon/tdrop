/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef NODEDOC_H_
#define NODEDOC_H_

#include "xmlFile.h"

class xmlFile;

class nodeDoc {
public:
	/**
	 * Create a new nodeDoc
	 * @param xf The xml file
	 * @param nd The parent node name. This node should be accessible from root. 
	 */
	nodeDoc(xmlFile *xf, string nd);
	
	/**
	 * Create a new nodeDoc
	 * @param xf The xml file
	 * @param nd The parent node name. This node should be accessible from parent.
	 * @param parent the nodeDoc parent for nd. 
	 */
	nodeDoc(xmlFile *xf, string nd, nodeDoc parent);
	nodeDoc(const nodeDoc &);
	virtual ~nodeDoc();
	
	/**
	 * @return true if the current node exists in the file 
	 */
	bool isValid();
	
	/**
	 * Browses to next node having the specified name
	 */
	void next();
	
	/**
	 * @return the value of the current node
	 */
  string getNodeValue();

	/**
	 * @param name The attribute value
	 * @return the value of the current node's attribute name. Returns "" if none found.
	 */
	string getAttributeValueByName(string name);
	
	/**
	 * Sets the content of the current node. Create it if needed.
	 * @param c The new node value
	 */
	void setNodeContent(string c);
	
	/**
	 * Jump to the specified named current node's child
	 * @param nd The child name
	 */
	void findChildByName(string nd);
	
	/**
	 * Jump to the specified named child of n
	 * @param nd The child name
	 * @param n The nodeDoc to search from
	 */
	void findChildByName(string nd, nodeDoc *n);
	
	/**
	 * Jump to the first child of the current node
	 */
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
