/** Teardrop - validPages.h

@brief This class enables the interface to create threads validating links

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 28/10/2006

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

#ifndef VALIDPAGES_H
#define VALIDPAGES_H

#include <iostream>
#include "net/getHttp.h"
#include "pthread.h"

using namespace std;

class validPages {
private:
    string url;
    bool *results;
    tdParam *tdp;
public:
    validPages(string u,bool *r, tdParam *t);
    void validate();
	void operator()() { validate(); }
};

#endif
