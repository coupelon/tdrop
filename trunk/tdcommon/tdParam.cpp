/** Teardrop - tdParam.cpp

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

#include "tdParam.h"

tdParam::tdParam() {
    echopages = false;
    browser = "";
    proxy_address = "";
    proxy_port = 0;
    proxy_type = CURLPROXY_HTTP;
    curl_timeout = CURL_TIMEOUT;
    getInitValues();
}

void tdParam::setBrowser(string b) {
  browser = b;
}

string tdParam::getBrowser() {
  return browser;
}

void tdParam::setEcho(bool e) {
    echopages = e;
}

void tdParam::setProxy(string a,string p,string t) {
    proxy_address = a;
    proxy_port = atol(p.c_str());
    proxy_type = (t == "SOCKS5")?CURLPROXY_SOCKS5:CURLPROXY_HTTP;
}

void tdParam::setTimeout(string t) {
    curl_timeout = atol(t.c_str());
}

bool tdParam::getEcho() {
    return echopages;
}

string tdParam::getProxyAddress() {
    return proxy_address;
}

long tdParam::getProxyPort() {
    return proxy_port;
}

long tdParam::getProxyType() {
    return proxy_type;
}

long tdParam::getTimeout() const {
    return curl_timeout;
}

void tdParam::getInitValues() {
  //Setting proxy settings
  xmlFile xf;
  string path;
  if (selectFile::find("config",".xml",path) && xf.openFile(path + "config.xml"))
  {
    nodeDoc *n = new nodeDoc(&xf,"browser");
    if (n->isValid())
    {
      browser = n->getNodeValue();
    }
    n->findChildByName("proxy-address",NULL);
    if (n->isValid())
    {
      proxy_address = n->getNodeValue();
    }
    n->findChildByName("proxy-port",NULL);
    if (n->isValid())
    {
      proxy_port = (n->getNodeValue() == "SOCKS5")?CURLPROXY_SOCKS5:CURLPROXY_HTTP;
    }
    n->findChildByName("proxy-type",NULL);
    if (n->isValid())
    {
      proxy_type = atol(n->getNodeValue().c_str());
    }
    n->findChildByName("timeout",NULL);
    if (n->isValid())
    {
      curl_timeout = atol(n->getNodeValue().c_str());
    }
    delete n;
  }
}

void tdParam::commit() {
  xmlFile xf;
  string path;
  if (selectFile::find("config",".xml",path) && xf.openFile(path + "config.xml"))
  {
  	nodeDoc n(&xf,"browser");
	n.setNodeContent(getBrowser());
	n.findChildByName("proxy-address",NULL);
	n.setNodeContent(getProxyAddress());
	n.findChildByName("proxy-port",NULL);
	n.setNodeContent(xmlFile::ltoa(getProxyPort()));
    n.findChildByName("proxy-type",NULL);
    n.setNodeContent((getProxyType()==CURLPROXY_HTTP)?"HTTP":"SOCKS5");
    n.findChildByName("timeout",NULL);
    n.setNodeContent(xmlFile::ltoa(getTimeout()));

    string out = selectFile::getHomeDirectory() + "config.xml";
    xf.saveDocTo(out);
  }
}

