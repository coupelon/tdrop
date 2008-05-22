/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "tdParam.h"

log4cxx::LoggerPtr tdParam::logger = log4cxx::Logger::getLogger("Teardrop");

tdParam::tdParam() {
    echopages = false;
    browser = "";
    proxy_address = "";
    proxy_port = 0;
    proxy_type = CURLPROXY_HTTP;
    curl_timeout = CURL_TIMEOUT;
    max_threads = THMAX;
    selectFile::createDirectoryStructure();
    getInitValues();
	  log4cxx::PropertyConfigurator::configure(selectFile::getHomeDirectory() + LOG_CONFIG);
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

void tdParam::setMaxThreads(string t){
	max_threads = atol(t.c_str());
}

long tdParam::getMaxThreads() const {
	return max_threads;
}

void tdParam::getInitValues() {
  //Setting proxy settings
  xmlFile xf;
  string path;
  if (selectFile::find(CONFIG_FILE,path) && xf.openFile(path))
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
    n->findChildByName("maxthreads",NULL);
    if (n->isValid())
    {
      max_threads = atol(n->getNodeValue().c_str());
    }
    delete n;
  }
}

void tdParam::commit() {
  xmlFile xf;
  if (xf.openFile(selectFile::getHomeConfigFile())) {
  	nodeDoc n(&xf);
		n.setChildContent("browser",getBrowser());
		n.setChildContent("proxy-address", getProxyAddress());
		n.setChildContent("proxy-port",xmlFile::ltoa(getProxyPort()));
    n.setChildContent("proxy-type",(getProxyType()==CURLPROXY_HTTP)?"HTTP":"SOCKS5");
    n.setChildContent("timeout",xmlFile::ltoa(getTimeout()));
    n.setChildContent("maxthreads",xmlFile::ltoa(getMaxThreads()));
    
    xf.saveDocTo(selectFile::getHomeConfigFile()); 
  }
}

