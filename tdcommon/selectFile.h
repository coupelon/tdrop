/** Teardrop - selectFile.h

@brief This class is used to get the correct access path of a file

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 16/09/2006

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

#ifndef SELECTFILE_H
#define SELECTFILE_H

#include <fstream>
#include <string>
#include "debugmacro.h"

using namespace std;

class selectFile {
public:
    static bool find(string, string, string &);
    static string getHomeDirectory();
private:
    static bool fileexist(string);
};

#endif
