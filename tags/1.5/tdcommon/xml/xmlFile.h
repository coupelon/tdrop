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

  /** Saves the XML doc to the specified file */
  bool saveDocTo(string &);
  


  string filename;
  xmlDoc *doc;
  xmlNode *root_element;
  
  /** Convert an integer to a string */
  static string itoa(int);

  /** Convert a long to a string */
  static string ltoa(long);
};

#endif
