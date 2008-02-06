/** Teardrop - regExp.h

@brief This class is an interface to commonly used regular expressions stuffs

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 04/04/2006

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


#ifndef REGEXP_H
#define REGEXP_H

#include <string>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

class regExp {
public:
	static string replaceAll(string,string,string);
	regExp(string,string &);
	regExp(string);
	~regExp();
	void newPage(string &);
	void next();
	bool endOfMatch();
	string getMatch(int);
	int getMatchNumber();
private:
	regex *reg;
	boost::sregex_iterator *reg_iterator;
};

#endif
