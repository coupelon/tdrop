/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include "tdExcp.h"

using namespace std;

/**
 * This structure holds the parameters of an HTTP request.
 * Both post and get are present, as one might use both in a single request.
 */
struct methodParam {
  string post;
  string get;
};

/**
 * The address class is used to handle a custom HTTP request.
 */
class address {
public:
	/// The base url of the query. It's more convenient to make that variable public
	string url;
	/// The parameters of the query. Public for convenience.
	methodParam param;
	
	address() { url = ""; param.post = ""; param.get = ""; method = true; }
	void setGET() { method = true; }
	void setPOST() { method = false; }
	bool isGET() const { return method; }
	bool isPOST() const { return !method; }
	
	/**
	 * @return The full url, concatenated with the GET parameters
	 * when available. It also takes care of the interrogation mark.
	 */
	string getFullUrl() const {
		if (url == "" && param.post == "" && param.get == "") {
              throw tdExcp("Trying to execute an empty query (both url and parameters) !");
		} else if (url == "" && param.get != "") {
			return param.get;
		} else if (url != "" && param.get != "") {
			return url + "?" + param.get;
		}
		return url;
	}
	
	/**
	 * Check if that address is empty
	 * @return true if everything in this address is empty.
	 */
	bool isEmpty() const { return (url == "" && param.post == "" && param.get == ""); }
	
	bool operator==(address & ad) const {
		return ( ad.url == this->url && ad.param.get == this->param.get && ad.param.post == this->param.post);
	}
	
	/**
	 * Addresses can be compared. They are lexicaly compared by url, GET and POST parameters
	 */
	bool operator<(const address & a1) const {
		return ( a1.url + a1.param.get + a1.param.post < url + param.get + param.post);
	}
	
	/**
	 * Addresses can be used a string streams, so they are friendly to <<
	 */
	friend ostream& operator <<(ostream& o, const address& ad) {
		o << ad.getFullUrl();
		if (ad.isPOST()) o << " (POST: " <<  ad.param.post << ")";
		return o;
	}
private:
  bool method;
};

#endif
