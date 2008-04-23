/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
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
