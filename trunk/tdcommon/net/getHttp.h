/** Teardrop - getHttp.h

@brief This class deals with HTTP retrieval

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 26/02/2006

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

#ifndef GETHTTP
#define GETHTTP

#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>
#include <curl/curl.h>
#include <iconv.h>
#include <errno.h>
#include "../address.h"
#include "../regexp/regExp.h"
#include "../debugmacro.h"
#include "../tdParam.h"
#include "rawContainer.h"

using namespace std;

//This parameter differs from OS to OS, and I didn't found any
// hints how to guess it.
#ifndef HTTP_DEFAULT_CHARSET
#define HTTP_DEFAULT_CHARSET "UTF-8"
#endif

class getHttp {
private:
  CURL *curl;
  CURLM *curl_multi;
  CURLcode res;
  static int writer(char *data, size_t size, size_t nmemb, string *writerData);
  static int writerToChar(char *data, size_t size, size_t nmemb, rawContainer * writerData);
  curl_slist *slist;
  string m_proxyAddress;
  long m_proxyPort;
  long m_proxyType;
  long m_timeout;
  bool *abort;

public:
    getHttp();
    ~getHttp();
    string getPage(address &, string ch = "");
    bool getFile (address &, string file);
    bool validatePage (string & url);
    bool getRawData (string & url, rawContainer *r);
    string escape(const string &);
    string unescape(const string &);
    string moreUnescape(string, string =  HTTP_DEFAULT_CHARSET);
    string charsetConvert(string,string,string = HTTP_DEFAULT_CHARSET);
    string getCharset(string);
    void setAcceptCharset(string);
    void setParam(tdParam *tdp);
    void addHttpHeader(string);
    void setAbort(bool *val) { abort = val; }
    bool getAbort() { if (abort) return *abort; else return false; }
};

#endif
