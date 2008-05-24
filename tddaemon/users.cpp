/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "users.h"

users::users() {
	UIDSession::initRand();
}

users::~users() {
}

string users::authenticateUser(const string & username, const string & password, const char *host) {
	//TODO: Implement a real file based check
	if (host && validateCouple(username,password)) {
		string id = UIDSession::getID();
		addUser(id,string(host));
		return id;
	} 
	return "";
}

void users::addUser(const string & id, const string & host) {
	usr newUser;
	newUser.id = id;
	newUser.host = host;
	clients.push_back(newUser);
}

bool users::isValid(const char *id, const char *host) {
	if (id && host) {
		string idS = string(id);
		string hostS = string(host);
		for(list<usr>::iterator usrIt = clients.begin(); usrIt != clients.end(); ++usrIt) {
			if (usrIt->id == idS && usrIt->host == hostS) return true;
		}
	}
	LOG4CXX_INFO(tdParam::logger, "Invalid user detected: " << string(id?id:"(null)") << "," << string(host?host:"(null)"));
	return false;
}

bool users::validateCouple(const string & user, const string & pass) {
	ifstream passfile;
	string line;
	string filename = selectFile::getHomeDirectory() + PASS_FILE;
	passfile.open (filename.c_str(), fstream::in);
	if (passfile.is_open()) {
		while (!passfile.eof()) {
			getline(passfile,line);
			regExp r("([^:]+):(.+)");
			r.newPage(line);
		  if (!r.endOfMatch()) {
		  	if (user == r.getMatch(1) && pass == r.getMatch(2)) return true;
		  }
		}
	}
	return false;
}
