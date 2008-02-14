/** Teardrop - rawUrl.h

@brief This class enables the interface to create threads downloading raw url's (mainly images)

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

#ifndef RAWURL_H
#define RAWURL_H

#include <iostream>
#include <map>
#include "getHttp.h"
#include <QMainWindow>

using namespace std;

class rawUrl {
private:
    string url;
    map<string,rawContainer *> *results;
    //rawContainer *results;
    tdParam *tdp;
    //const wxWindow *parent;
public:
    rawUrl(string u,map<string,rawContainer *> *r, tdParam *t, QMainWindow *mw);
    void getRawPage();
	void operator()() { getRawPage(); }
};

#endif
