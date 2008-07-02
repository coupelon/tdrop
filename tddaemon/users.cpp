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
	if (host && validateCouple(username,password)) {
		string id = UIDSession::getID();
		addUser(id,string(host), username);
		LOG4CXX_INFO(tdParam::logger, "User " << username << " connected, ID: " << id << " (" << host << ")");
		return id;
	}
	LOG4CXX_WARN(tdParam::logger, "Failed to authenticate user " << username);
	return "";
}

void users::addUser(const string & id, const string & host, const string & name) {
	usr newUser;
	newUser.host = host;
	newUser.name = name;
	clients[id] = newUser;
}

bool users::isValid(const char *id, const char *host) {
	if (id && host) {
		string idS = string(id).substr(3);
		string hostS = string(host);
		if (clients.find(idS) != clients.end()) {
			if (clients[idS].host == hostS) return true;
		}
	}
	LOG4CXX_DEBUG(tdParam::logger, "Failed to verify cookie " << string(id?id:"(null)") << ":" << string(host?host:"(null)"));
	return false;
}

bool users::validateCouple(const string & user, const string & pass) {
	SHA1 sha;
	unsigned	message_digest[5];
	char shaChar[41];
	ifstream passfile;
	string line;
	string filename = selectFile::getHomeDirectory() + PASS_FILE;
	sha.Reset();
	sha << "Teardrop" << pass.c_str();
	if (!sha.Result(message_digest)) {
		LOG4CXX_ERROR(tdParam::logger, "Could not compute the password's digest");
	}
	sprintf(shaChar, "%08X%08X%08X%08X%08X",
					message_digest[0],
					message_digest[1],
					message_digest[2],
					message_digest[3],
					message_digest[4]);
	string shaString = "{SHA}";
	shaString += shaChar;
	passfile.open (filename.c_str(), fstream::in);
	if (passfile.is_open()) {
		while (!passfile.eof()) {
			getline(passfile,line);
			regExp r("([^:]+):(.+)");
			r.newPage(line);
		  if (!r.endOfMatch()) {
		  	if (user == r.getMatch(1) && shaString == r.getMatch(2)) {
		  	    passfile.close();
		  	    return true;
		  	}
		  }
		}
		passfile.close();
	} else {
		LOG4CXX_WARN(tdParam::logger, "Could not find password file " << selectFile::getHomeDirectory() << PASS_FILE);
	}
	LOG4CXX_INFO(tdParam::logger, "Invalid couple " << user << ":" << shaString);
	return false;
}

string users::getUsername(const string & id) {
	string idS = id.substr(3);
	if (clients.find(idS) != clients.end()) {
		return clients[idS].name;
	}
	return "";
}

string users::getUserHistory(const char *id, const char *host) {
  if (!isValid(id,host)) return "";
  DIR *dp;
  struct dirent *ep;
  string directory = selectFile::getHomeDirectory()
						+ HOME_SESSIONS
						+ getUsername(id)
						+ "/";
  dp = opendir (directory.c_str());
  if (dp != NULL) {
  string output = "";
    while ((ep = readdir(dp))) {
      engineResults er;
      if (openSave::xmlOpenHeader(directory + ep->d_name,&er)) {
        output += "{\"id\":\"";
        string fname = ep->d_name;
        output += fname.substr(0,fname.length()-4); //Remove the '.xml' suffix
			  output += "\",\"query\":\"";
			  output += er.getQuery();
			  output += "\",\"limit\":\"";
			  output += er.getLimit();
			  output += "\",\"engines\":[";
			  for (map<string, int>::const_iterator it = er.getEngines().begin(); it != er.getEngines().end(); ++it) {
			    output += "{\"name\":\"";
			    output += it->first;
			    output += "\"}";
			  }
			  output += "]}";
      }
      // puts (ep->d_name);
    }
    closedir (dp);
    return output;
  }
  else {
    LOG4CXX_INFO(tdParam::logger, "Couldn't load search list.");
  }
  return "";
}

bool users::authenticationRequired() {
  ifstream passfile;
	string filename = selectFile::getHomeDirectory() + PASS_FILE;
	passfile.open (filename.c_str(), fstream::in);
	if (passfile.is_open()) {
	    passfile.close();
	    return true;
	}
	return false;
}

