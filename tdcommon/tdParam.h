/** Teardrop - tdParam.h

@brief This holds important parameters of Teardrop

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 03/10/2006

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

#ifndef TDPARAM_H
#define TDPARAM_H

#include <curl/curl.h>
#include <string>
#include "xml/xmlFile.h"
#include "xml/nodeDoc.h"
#include "selectFile.h"

#define CURL_TIMEOUT 90

using namespace std;

class tdParam {
public:
    tdParam();
    
    void setEcho(bool);
    void setProxy(string,string,string);
    
    bool getEcho();
    string getProxyAddress();
    long getProxyPort();
    long getProxyType();
    void setTimeout(string t);
    long getTimeout() const;
    string getBrowser();
    void setBrowser(string b);
    void commit();
private:
    bool echopages;
    string proxy_address;
    long proxy_port;
    long proxy_type;
    long curl_timeout;
    string browser;
    void getInitValues();
};

#endif
