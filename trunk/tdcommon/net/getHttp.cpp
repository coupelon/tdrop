/** Teardrop - getHttp.cpp

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
    curl_easy_setopt (curl, CURLOPT_NOBODY);
    
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
        string url = ad.getFullUrl();
        if (getRawData(url,&cont)) {
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
	string buffer;
	string url;
	if (curl) {
		curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt (curl, CURLOPT_COOKIEFILE, "");
		curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt (curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt (curl, CURLOPT_TIMEOUT, m_timeout);
		debug(ad.getFullUrl().c_str());
		//This copy to url must be done, instead of what you might get a malformed url error from curl
		url = ad.getFullUrl ();
		//		cout << url <<  endl;
		char *buffer_url = (char *) calloc(2048,1);
		strncpy(buffer_url,url.c_str(),2047);
		//curl_easy_setopt (curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt (curl, CURLOPT_URL, buffer_url);
		if (ad.isPOST ()) {
			url = ad.param.post;
			curl_easy_setopt (curl, CURLOPT_POSTFIELDS, url.c_str ());
			curl_easy_setopt (curl, CURLOPT_POST);
		} else curl_easy_setopt (curl, CURLOPT_HTTPGET);
    if (m_proxyAddress != "") {
        curl_easy_setopt(curl, CURLOPT_PROXY, m_proxyAddress.c_str());
        curl_easy_setopt(curl, CURLOPT_PROXYPORT, m_proxyPort);
        curl_easy_setopt(curl, CURLOPT_PROXYTYPE, m_proxyType);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    curl_multi_add_handle(curl_multi,curl);
    int still_running = 1;
    
    //while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(curl_multi, &still_running));
    
    while(still_running) {
      struct timeval timeout;
      int rc; /* select() return code */

      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      int maxfd;

      FD_ZERO(&fdread);
      FD_ZERO(&fdwrite);
      FD_ZERO(&fdexcep);

      /* set a suitable timeout to play around with */
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;

      /* get file descriptors from the transfers */
      curl_multi_fdset(curl_multi, &fdread, &fdwrite, &fdexcep, &maxfd);

      rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);

      switch(rc) {
      case -1:
        /* select error */
        break;
      case 0:
        debug("timeout!\n");
      default:
        /* timeout or readable/writable sockets */
        //printf("perform!\n");
        while(CURLM_CALL_MULTI_PERFORM ==
              curl_multi_perform(curl_multi, &still_running) && !getAbort());
        //printf("running: %d!\n", still_running);
        if (getAbort()) {
          free(buffer_url);
          return "";
        }
        break;
      }
    }

    res = curl_multi_info_read(curl_multi,&still_running)->data.result;
    curl_multi_remove_handle(curl_multi,curl);
		//res = curl_easy_perform (curl);
		free(buffer_url);
		if (res != CURLE_OK) {
			cerr << "This page could'nt be retrieved : " << ad << endl;
			return "";
		}
		if (getAbort()) return "";
		return charsetConvert(buffer,getCharset(ch));
	}
	cerr << "Curl was not ready to download : " << ad << endl;
	return "";
}

bool getHttp::getRawData (string & url, rawContainer *r) {
    if (curl) {
        curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, writerToChar);
		curl_easy_setopt (curl, CURLOPT_WRITEDATA, r);
        curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt (curl, CURLOPT_TIMEOUT, m_timeout);
        
        curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt (curl, CURLOPT_COOKIEFILE, "");

        curl_easy_setopt (curl, CURLOPT_URL, url.c_str ());
        curl_easy_setopt (curl, CURLOPT_HTTPGET);
        
        if (m_proxyAddress != "") {
            curl_easy_setopt(curl, CURLOPT_PROXY, m_proxyAddress.c_str());
            curl_easy_setopt(curl, CURLOPT_PROXYPORT, m_proxyPort);
            curl_easy_setopt(curl, CURLOPT_PROXYTYPE, m_proxyType);
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        res = curl_easy_perform (curl);

        long code;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &code);
        if (code != 404 && res == CURLE_OK) return true;
    }
    return false;
}


string getHttp::charsetConvert(string s, string from_charset, string to_charset) {    
    to_charset += "//IGNORE";
    
    if (from_charset == "" || to_charset == "") return s;
    size_t size1 = s.size();
    
    char *buf1 = new char[size1];
    //char *buf1cop = buf1;
    strncpy(buf1,s.c_str(),size1 );
    
    size_t size2 = (size1+1)*2;
    
    char *buf2 = new char[size2+2];
    char *buf2cop = buf2;
    memset(buf2, 0, sizeof(char)*(size2+2));
    iconv_t trans = iconv_open(to_charset.c_str(), from_charset.c_str());
    
    if (trans == (iconv_t)(-1)) {
        debug("A problem with the choosen charset occured ! (from %s to %s",from_charset.c_str(),to_charset.c_str());
        return s;
    }
    
    #ifdef _LIBICONV_W32_H
        size_t res = iconv(trans, (const char **) &buf1, &size1, &buf2, &size2);
    #else
        size_t res = iconv(trans, &buf1, &size1, &buf2, &size2);
    #endif
    
    iconv_close(trans);
    if (res == (size_t) -1) {
        debug("A conversion error occured, from %s to %s ",from_charset.c_str(),to_charset.c_str());
        return s;
        
    }
    return buf2cop;
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
    return curl_escape(s.c_str(),s.size());
    //return curl_easy_escape(curl,s.c_str(),s.size());
}

string getHttp::unescape(const string & s) {
    return curl_unescape(s.c_str(),s.size());
    //return curl_easy_unescape(curl,s.c_str(),s.size());
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
