/** Teardrop - xmlFile.h

@brief This class provides the basic functions to parse an xml file.

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

#ifndef XMLFILE_H
#define XMLFILE_H

#include <iostream>
#include <sstream>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace std;

class xmlFile {
public:
  xmlFile();
  ~xmlFile();

  /** This method tries to open the given file
  Returns true in case of success */
  bool openFile(string);
  void close();

public:
  /** This method tries to find the given tag, by its name,
  in the children of the given xmlNode */
  xmlNode *findChildByName(xmlNode *,string);

  /** This method tries to find the given tag, by its name,
  after the given xmlNode */
  xmlNode *findNodeByName(xmlNode *,string);

  /** This method tries to find the given tag, by its name,
  before the given xmlNode */
  xmlNode *findPreviousNodeByName(xmlNode *,string);

  /** This method tries to find the given attribute, by its name,
  in the given xmlNode */
  xmlAttr *getAttributeByName(xmlNode *,string);

  /** Returns the value of the given node's attribute name.
  Returns "" if none found. */
  string getAttributeValueByName(xmlNode *n, string name);

  /** Returns the value of the given node */
  string getNodeValue(xmlNode *);

  /** Saves the XML doc to the specified file */
  bool saveDocTo(string &);
  
  /** Sets the content of the given node. Create it if needed */
  void setNodeContent(xmlNode *, string, string);

  string filename;
  xmlDoc *doc;
  xmlNode *root_element;
  
  /** Convert an integer to a string */
  static string itoa(int);

  /** Convert a long to a string */
  static string ltoa(long);
};

#endif
