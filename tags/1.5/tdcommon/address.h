/** Teardrop - address.h

@brief This class represents an address, with the options relevant the software

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

#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include "tdExcp.h"

using namespace std;

struct methodParam {
  string post;
  string get;
};

class address {
public:
	string url;
	methodParam param;
	// Only two options, POST or GET.
	bool method;
	address() { url = ""; param.post = ""; param.get = ""; method = true; }
	void setGET() { method = true; }
	void setPOST() { method = false; }
	bool isGET() const { return method; }
	bool isPOST() const { return !method; }
	string getFullUrl() const {
		if (url == "" && param.post == "" && param.get == "") {
              throw tdExcp("Trying to execute an empty query (both url and parameters) !");
		} else if (url == "" && param.get != "") {
			return param.get;
		} else if (url != "" && param.get != "") {
			return url + "?" + param.get;
		}
		return url;
	}
	bool isEmpty() const { return (url == "" && param.post == "" && param.get == ""); }
	bool operator==(address & ad) const {
		return ( ad.url == this->url && ad.param.get == this->param.get && ad.param.post == this->param.post);
	}
	bool operator<(const address & a1) const {
		return ( a1.url + a1.param.get + a1.param.post < url + param.get + param.post);
	}
	friend ostream& operator <<(ostream& o, const address& ad) {
		o << ad.getFullUrl();
		if (ad.isPOST()) o << " (POST: " <<  ad.param.post << ")";
		return o;
	}
};

#endif
