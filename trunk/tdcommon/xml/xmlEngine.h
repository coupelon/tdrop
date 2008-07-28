/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef XMLENGINE_H
#define XMLENGINE_H

#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "nodeDoc.h"
#include "../regexp/regExp.h"
#include "../address.h"
#include "../net/getHttp.h"
#include "../selectFile.h"
#include "../rowContainer.h"

using namespace std;

class xmlEngine {
public:
	xmlEngine();
	~xmlEngine();

	/** Open an engine definition
	 * @param f The engine raw name, i.e. engine filename without '.xml'.
   */
  bool openEngine(string f);

  /**
   * Returns the name of the current search engine
   */      
	string getName();
	
	/**
	 * Returns the charset expected to be used by the search engine
	 * It is used when the returned content header doesn't contain this information
	 * If it is not referenced here and no content information is given by the
	 * engine, the content is passed directly to the application
   */
	string getCharset();

  /**
   * Returns the separator to be used in the query to split words
   */
  string getSeparator();
  
  string getResultFieldName();
  int getResultFieldNumber();
  bool getResultFieldComparable();
  bool getResultFieldStripHTML();
  bool getResultFieldEscape();
  string getResultFieldValue();
  void initResultField();

  
  /**
   * This method increments the result tag pointer to it's next field block
   * @return true in case of success.
   */
  bool getNextResultField();
        
  /**
   * @return the regular expression of the result tag
   */
  string getResultRegExp();
  
  /**
   * @return the regular expression of the init tag
   */
  string getInitRegExp();

  /**
   * @return the regular expression of the nextinput tag
   */
  string getNextInputRegExp();      
  
  /**
   * This method increments the query tag pointer to it's next block
   * @return true in case of success.
   */
  bool getNextQuery();

  /**
   * This method increments the init tag pointer to it's next block
   * @return true in case of success.
   */
  bool getNextInit();      
  
  /**
   * @param res The query information
   * @param r The regular expression from the init block
   * @return the init complete address
   */
  address getInitAddress(rowContainer *res, regExp *r);

  /**
   * @param res The query information
   * @param r The regular expression from the query block
   * @return the query complete address
   */
  address getQueryAddress(rowContainer *res, regExp *r);      
  
  /**
   * @param res The query information
   * @param reg The regular expression from the nextinput block
   * @param r The number of results needed to produce the address
   * @return the nextinput complete address
   */
	address getNextInputAddress(rowContainer *res, regExp *reg, int r);

  /**
   * The nextinput tag is optional. This function checks for its presence
   * @return true if it exists
   */
  bool hasNextInput();
  
  /**
   * Sets the gh attribute. Needed by getNextInputFields for charaters encoding
   */
  void setHttp(getHttp *);
  
  /**
   * This method passes the internal header pointer to the next one.
   * @return true if it exists 
   */
  bool getNextHeader();
  
  /**
   * @return the currently pointed header
   */
  string getHeader();
  
  /**
   * Get the optional tag "updatable" 
   * @return false if set to 'no'
   */
  bool shouldUpdate();
  
  /**
   * Get the optional url that should be used to update the engine
   * @return the engine url
   */
  string getUpdateUrl();
      
private:

  /** Returns the current init's tag url */
  string getInitUrl();

  /** Returns the current querie's tag url */
  string getQueryUrl();

  /** Returns the current nextinput's tag url */
  string getNextInputUrl();

  /** Generic function used to retrieve a url from a given tag */
  string getUrl(nodeDoc *);

  /** Returns the value of the given init inputfield */
  methodParam getInitInputFields(rowContainer *, regExp *, bool methodGet);
  
  /** Returns the value of the given query inputfield */
  methodParam getQueryInputFields(rowContainer *, regExp *, bool methodGet);

  /** Returns the value of the given nextinput inputfield */
  methodParam getNextInputFields(rowContainer *, regExp *, bool methodGet, int res);      
  
  /** This method generate the value of a given inputfield */
  methodParam getInputFields(rowContainer *, regExp *,nodeDoc *,bool methodGet, int res = -1);

  /** Returns the input method of the init block */
  string getInitMethod();
  
  /** Returns the input method of the query block */
  string getQueryMethod();

  /** Returns the input method of the nextinput block */
  string getNextInputMethod();
  
  /** Returns the input method of the given block */
  string getMethod(nodeDoc *);
  
  /** Generates the query based on the input, charset, and the separator */
  string generateQuery(rowContainer *res);

	nodeDoc *init;
	nodeDoc *query;
  nodeDoc *result;
  nodeDoc *header;
  
  xmlFile *xml;

  getHttp *gh;
};

#endif
