/** Teardrop - xmlEngine.h

@brief This class is used to parse the xml engines files.

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 09/07/2006

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

#ifndef XMLENGINE_H
#define XMLENGINE_H

#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "xmlFile.h"
#include "../regexp/regExp.h"
#include "../address.h"
#include "../net/getHttp.h"
#include "../selectFile.h"
#include "../engineResults.h"

using namespace std;

class xmlEngine : public xmlFile {
	public:
        /** xmlEngine constructor. Takes the engines name
        for parameter, namely the engine filename without it's path
        and .xml.
        */
		xmlEngine();
		~xmlEngine();

    bool openEngine(string);

    /** Returns the name of the current search engine */      
		string getName();
		
		/** Returns the charset expected to be used by the search engine
		It is used when the returned content header doesn't contain this information
		If it is not referenced here and no content information is given by the
		engine, the content is passed directly to the application
        */
		string getCharset();

    /** Returns the separator to be used in the query to split words */
    string getSeparator();
    
    string getResultFieldName();
    int getResultFieldNumber();
    bool getResultFieldComparable();
    bool getResultFieldStripHTML();
    bool getResultFieldEscape();
    string getResultFieldValue();
    void initResultField();

    
    /** This method increments the result tag pointer to it's next field block
    Returns true in case of success.
        */
    bool getNextResultField();
          
    /** Returns the regular expression of the result tag */
    string getResultRegExp();
    
    /** Returns the regular expression of the init tag */
    string getInitRegExp();

    /** Returns the regular expression of the nextinput tag */
    string getNextInputRegExp();      
    
    /** This method increments the query tag pointer to it's next block
    Returns true in case of success.
    */
    bool getNextQuery();

    /** This method increments the init tag pointer to it's next block
    Returns true in case of success.
      */
    bool getNextInit();      
    
    /** returns the init complete address */
    address getInitAddress(engineResults *, regExp *);

    /** returns the query complete address */
    address getQueryAddress(engineResults *, regExp *);      
    
    /** returns the nextinput complete address */
		address getNextInputAddress(engineResults *, regExp *, int);

    /** As the nextinput tag is optional, this function returns true if
    it exists */
    bool hasNextInput();
    
    /** Sets the gh attribute. Needed by getNextInputFields */
    void setHttp(getHttp *);
    
    /** This method passes the internal header pointer to the next one */
    bool getNextHeader();
    
    /** Returns the currently pointed header */
    string getHeader();
    
    /** Optional: If tag "updatable" is set to no, return false */
    bool shouldUpdate();
    
    /** Optional: Returns the url used to update the engine */
    string getUpdateUrl();
        
	private:

    /** Returns the current init's tag url */
    string getInitUrl();

    /** Returns the current querie's tag url */
    string getQueryUrl();

    /** Returns the current nextinput's tag url */
    string getNextInputUrl();

    /** Generic function used to retrieve a url from a given tag */
    string getUrl(xmlNode *);

    /** This method increments a given tag pointer to it's next block */
    bool getNext(xmlNode **, string, xmlNode * = NULL);

    /** Returns the value of the given init inputfield */
    methodParam getInitInputFields(engineResults *, regExp *, bool methodGet);
    
    /** Returns the value of the given query inputfield */
    methodParam getQueryInputFields(engineResults *, regExp *, bool methodGet);

    /** Returns the value of the given nextinput inputfield */
    methodParam getNextInputFields(engineResults *, regExp *, bool methodGet, int res);      
    
    /** This method generate the value of a given inputfield */
    methodParam getInputFields(engineResults *, regExp *,xmlNode *,bool methodGet, int res = -1);

    /** Returns the input method of the init block */
    string getInitMethod();
    
    /** Returns the input method of the query block */
    string getQueryMethod();

    /** Returns the input method of the nextinput block */
    string getNextInputMethod();
    
    /** Returns the input method of the given block */
    string getMethod(xmlNode *);

		xmlNode *init;
		xmlNode *query;
        xmlNode *result;
        xmlNode *header;

        getHttp *gh;
};

#endif
