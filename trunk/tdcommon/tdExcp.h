/** Teardrop - tdExcp.h

@brief This class is an exception used by teardrop.

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 10/10/2006

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

#ifndef TDEXCP_H
#define TDEXCP_H

#include <exception>
using namespace std;

class tdExcp: public exception {
public:
  tdExcp(string s) : exception() {
      error = s;
  }
  ~tdExcp() throw() {};
  virtual const char* what() const throw ()
  {
    return error.c_str();
  }
private:
  string error;
};

#endif
