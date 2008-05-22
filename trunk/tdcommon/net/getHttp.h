/*
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
#include "../tdParam.h"
#include "../address.h"
#include "../regexp/regExp.h"
#include "rawContainer.h"

using namespace std;

//This parameter differs from OS to OS, and I didn't found any
// hints how to guess it.
#ifndef HTTP_DEFAULT_CHARSET
#define HTTP_DEFAULT_CHARSET "UTF-8"
#endif

/**
 * The HTTP processing class. It is a basic frontend to libcurl.
 */

class getHttp {

public:
  getHttp();
  ~getHttp();
  
  /**
   * Retrieves a page
   * @param ad The address of the page
   * @param ch The charset the page is encoded in
   * @return the page as a string. "" if something went wrong
   */
  string getPage(address & ad, string ch = "");
  
  /**
   * Retrieve a binary file
   * @param ad The address of the file
   * @param file The destination file
   * @return true if everything was ok 
   */
  bool getFile (address & ad, string file);
  
  /**
   * Retrieve a binary url content in memory
   * @param url The address of the file
   * @param r The destination rawContainer
   * @return true if everything was ok 
   */
  bool getRawData (address & ad, rawContainer *r);
  
  /**
   * @return true if the given url is pointing to an existing file.
   */
  bool validatePage (string & url);
  
  /**
   * Escapes a string with HTML characters
   * @param s The string to escape
   * @return The escaped string
   */
  string escape(const string & s);
  
  /**
   * Unescapes a string from its HTML characters
   * @param s The string to unescape
   * @return The unescaped string
   */
  string unescape(const string & s);
  
  /**
   * Translates the HTML characters '&#xxx;' to a given charset
   * @param s The string to unescape
   * @param to_charset The goal charset
   * @return The unescaped string
   */
  string moreUnescape(string s, string to_charset = HTTP_DEFAULT_CHARSET);
  
  /**
   * Converts a string from one charset to another
   * @param s The string to convert
   * @param from_charset The original charset.
   * @param to_charset The goal charset
   * @return The converted string
   */
  string charsetConvert(string s, string from_charset, string to_charset = HTTP_DEFAULT_CHARSET);
  
  /**
   * Determine the charset used in the answer of a page request.
   * This method checks the content-type returned by the server,
   * and returns the given parameter if nothing is found
   * @param ch A guessed charset (either already known, or default)
   * @return The charset
   */
  string getCharset(string ch);
  
  /**
   * Add the "Accept-Charset:" header to the HTTP request
   * @param the accepted charset, given as an iconv string (see iconv -l)
   */
  void setAcceptCharset(string);
  
  /**
   * Sets the connection parameters
   * @param tdp The Teardrop's parameters
   */
  void setParam(tdParam *tdp);
  
  /**
   * Special headers can be specified using the header tag in the
   * xml definition, and this method allows us to add a new one
   * @param The header to add
   */
  void addHttpHeader(string h);
  
  /**
   * @param val If set to true, tells getHttp to stop processing as soon as possible
   */
  void setAbort(bool *val) { abort = val; }
  bool getAbort() { if (abort) return *abort; else return false; }
    
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
};

#endif
