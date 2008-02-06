/** Teardrop - openSave.h

@brief This class manages the all process of loading
@brief and saving teardrop's files

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 07/08/2006

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

#ifndef OPENSAVE_H
#define OPENSAVE_H

#include <iostream>
#include <fstream>
#include <set>
#include "row.h"
#include "net/getHttp.h"
#include "xml/xmlFile.h"
#include "regexp/regExp.h"
using namespace std;

class openSave {
public:
    //This method saves a query in a file.
    static bool xmlSave(string, string, list<string>, string, vector<row>);
    
    //This method loads a previously saved file.
    static bool xmlOpen(string, string &, list<string> &, string &, vector<row> &);
    
    //This method saves a query in a file (HTML).
    static bool htmlExport(string, string, list<string>, string, vector<row>);
    
    //This method saves a query in a file (CSV).
    static bool csvExport(string file, vector<row>);
};

#endif
