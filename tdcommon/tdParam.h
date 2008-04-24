/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
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
    
    /**
     * This method sets Teardrop to echo every pages that it retrieves from the web
     * useful for creating new engines.
     * @param e Set to true to activate echoing
     */
    void setEcho(bool e);
    
    /**
     * @param a address of the proxy server
     * @param p port number of the proxy server
     * @param t type of proxy ("SOCKS5" or "HTTP")
     */
    void setProxy(string a,string p,string t);
    
    bool getEcho();
    string getProxyAddress();
    long getProxyPort();
    long getProxyType();
    
    /**
     * Sets the timeout of pages
     * @param t The timeout in seconds 
     */
    void setTimeout(string t);
    long getTimeout() const;
    string getBrowser();
    
    /**
     * Sets the browser command line to execute
     * @param b The command line to launch the browser
     */
    void setBrowser(string b);
    
    /**
     * Write down the configuration modifications to teh Teardrop's
     * configuration file.
     */
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
