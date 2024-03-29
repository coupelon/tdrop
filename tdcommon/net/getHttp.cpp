/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "getHttp.h"

getHttp::getHttp () {
  curl_multi = curl_multi_init();
	curl = curl_easy_init ();
	slist = NULL;
  m_proxyAddress = "";
  m_timeout = CURL_TIMEOUT;
  abort = NULL;
}

getHttp::~getHttp () {
  if (slist)                  
      curl_slist_free_all(slist);
	curl_easy_cleanup(curl);
	curl_multi_cleanup(curl_multi);
}

void getHttp::setAcceptCharset(string charset) {
  string ac = "Accept-Charset: ";     
  if (charset != "")
  	ac += charset;
	else
  	ac += HTTP_DEFAULT_CHARSET;
  slist = curl_slist_append(slist, ac.c_str());
}

void getHttp::addHttpHeader(string h) {
  slist = curl_slist_append(slist, h.c_str());
}

void getHttp::setParam(tdParam *tdp) {
  m_proxyAddress = tdp->getProxyAddress();
  m_proxyPort = tdp->getProxyPort();
  m_proxyType = tdp->getProxyType();
  m_timeout = tdp->getTimeout();
}

int getHttp::writer(char *data, size_t size, size_t nmemb, string * writerData) {
	if (writerData == NULL)
		return 0;
    writerData->append(data, size * nmemb);
	return size * nmemb;
}

int getHttp::writerToChar(char *data, size_t size, size_t nmemb, rawContainer * writerData) {
	if (writerData == NULL)
		return 0;
    writerData->append(data, size * nmemb);
	return size * nmemb;
}

bool getHttp::validatePage (string & url) {
  if (curl) {        
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt (curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt (curl, CURLOPT_TIMEOUT, m_timeout);

    curl_easy_setopt (curl, CURLOPT_URL, url.c_str ());
    curl_easy_setopt (curl, CURLOPT_NOBODY,1);
    
    if (m_proxyAddress != "") {
      curl_easy_setopt(curl, CURLOPT_PROXY, m_proxyAddress.c_str());
      curl_easy_setopt(curl, CURLOPT_PROXYPORT, m_proxyPort);
      curl_easy_setopt(curl, CURLOPT_PROXYTYPE, m_proxyType);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    res = curl_easy_perform (curl);

    long code;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &code);
    if ((!(code >= 400 && code < 500) || code == 421) && res == CURLE_OK) return true;
  }
  return false;
}

bool getHttp::getFile (address & ad, string file) {
    FILE *output = fopen(file.c_str(),"wb");
    if (output) {
        rawContainer cont;
        if (getRawData(ad,&cont)) {
            fwrite(cont.getContent(),1,cont.getLength(),output);
            fclose(output);
            return true;
        }
        fclose(output);
    }
    return false;
}

string getHttp::getPage (address & ad, string ch)
{
	rawContainer rc;
	if (!getRawData(ad,&rc)) return "";
	return charsetConvert(string(rc.getContent(),rc.getLength()),getCharset(ch));
}

bool getHttp::getRawData (address & ad, rawContainer *r) {
	string url;
	if (!curl) return false;
	curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt (curl, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, writerToChar);
	curl_easy_setopt (curl, CURLOPT_WRITEDATA, r);
  curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt (curl, CURLOPT_TIMEOUT, m_timeout);
	LOG4CXX_DEBUG(tdParam::logger,ad.getFullUrl());
	//This copy to url must be done, instead of what you might get a malformed url error from curl
	url = ad.getFullUrl ();
	char *buffer_url = (char *) calloc(2048,1);
	strncpy(buffer_url,url.c_str(),2047);
	//curl_easy_setopt (curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt (curl, CURLOPT_URL, buffer_url);
	if (ad.isPOST ()) {
		url = ad.param.post;
		curl_easy_setopt (curl, CURLOPT_POSTFIELDS, url.c_str ());
		curl_easy_setopt (curl, CURLOPT_POST,1);
	} else curl_easy_setopt (curl, CURLOPT_HTTPGET,1);
  if (m_proxyAddress != "") {
    curl_easy_setopt(curl, CURLOPT_PROXY, m_proxyAddress.c_str());
    curl_easy_setopt(curl, CURLOPT_PROXYPORT, m_proxyPort);
    curl_easy_setopt(curl, CURLOPT_PROXYTYPE, m_proxyType);
  }
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
  curl_multi_add_handle(curl_multi,curl);
  int still_running = 1;
  
  while(still_running) {
    long tmout = 0;
    curl_multi_timeout(curl_multi, &tmout);
    if (tmout != 0) {
      LOG4CXX_DEBUG(tdParam::logger, "Supposed to wait socket for " + xmlFile::ltoa(tmout) + "ms");
      if (tmout < 0) { 
        tmout = 1000; // if libcurl returns a -1 timeout here, it just means that libcurl currently has no stored timeout value. You must not wait too long (more than a few seconds perhaps) before you call curl_multi_perform() again. 
      } else {
        tmout = tmout % 500; // Just don't wait more than 500ms, even if requested. 'Correct' curl_multi_timeout behavior under windows
      }
    #ifdef WIN32
      Sleep(tmout);
    #else
      usleep(tmout*1000);
    #endif
    }
    while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(curl_multi, &still_running) && !getAbort());
    if (getAbort()) {
      free(buffer_url);
      return false;
    }
  }

  res = curl_multi_info_read(curl_multi,&still_running)->data.result;
  curl_multi_remove_handle(curl_multi,curl);
	free(buffer_url);
	if (res != CURLE_OK) {
	  LOG4CXX_DEBUG(tdParam::logger,"Failed to retrieve " + ad.getFullUrl());
		return false;
	}
	if (getAbort()) return false;
	LOG4CXX_DEBUG(tdParam::logger,"Successfully retrieved " + ad.getFullUrl());
	return true;
}


string getHttp::charsetConvert(string s, string from_charset, string to_charset) {    
    to_charset += "//IGNORE";
    
    if (from_charset == "" || to_charset == "") return s;
    size_t size1 = s.size();
    
    char *buf1 = new char[size1];
    char *buf1cop = buf1;
    strncpy(buf1,s.c_str(),size1 );
    
    size_t size2 = (size1+1)*2;
    
    char *buf2 = new char[size2+2];
    char *buf2cop = buf2;
    memset(buf2, 0, sizeof(char)*(size2+2));
    iconv_t trans = iconv_open(to_charset.c_str(), from_charset.c_str());
    
    if (trans == (iconv_t)(-1)) {
        LOG4CXX_WARN(tdParam::logger,"A problem with the choosen charset occured ! (from " + from_charset + " to " + to_charset);
        return s;
    }
    
    #ifdef WIN32
        size_t res = iconv(trans, (const char **) &buf1, &size1, &buf2, &size2);
    #else
        size_t res = iconv(trans, &buf1, &size1, &buf2, &size2);
    #endif
    
    iconv_close(trans);
    delete buf1cop;
    string output(buf2cop);
    delete buf2cop;
    //if (buf2) delete buf2;
    if (res == (size_t) -1) {
         LOG4CXX_WARN(tdParam::logger,"A conversion error occured, from " + from_charset + " to " + to_charset);
        return s;
        
    }
    return output;
}

string getHttp::getCharset(string ch) {
    //Determine the charset used in the answer
    char *content;
    curl_easy_getinfo (curl, CURLINFO_CONTENT_TYPE, &content);
    string charset = "";
    if (ch != "") charset = ch;
    if (content) {
        string sc = content;
        transform(sc.begin(),sc.end(),sc.begin(),(int (*)(int)) toupper);
        regExp r("CHARSET\\s*=\\s*[\"]{0,1}([^;\"]*)",sc);
        if (!r.endOfMatch()) {
            charset = r.getMatch(1);
        }
    }
    return charset;
}

string getHttp::escape(const string & s) {
  char *outc = curl_easy_escape(curl,s.c_str(),s.size());
  string output(outc);
  curl_free(outc);
  return output;
}

string getHttp::unescape(const string & s) {
	int num;
  char *outc = curl_easy_unescape(curl,s.c_str(),s.size(),&num);
  string output(outc,num);
  curl_free(outc);
  return output;
}

string getHttp::moreUnescape(string s, string to_charset) {
  regExp r(charsetConvert("&#([0-9]+);","ISO-8859-1",to_charset));
  string::size_type pos = 0;
  for(r.newPage(s); !r.endOfMatch(); r.next()) {
    pos = s.find(r.getMatch(0),pos);
    string rep = r.getMatch(1);
    unsigned char c = 0;
    for (unsigned int i = 0; i < rep.size(); ++i) c = 10*c + (rep[i]-'0');
    rep = c;
    rep = charsetConvert(rep,"ISO-8859-1",to_charset);
    s = s.replace(pos,r.getMatch(0).size(),rep);
	}
  return s;
}
