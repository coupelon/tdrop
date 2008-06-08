/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/


#ifndef USERS_H_
#define USERS_H_

#include <iostream>
#include <list>
#include "tdParam.h"
#include "UIDSession.h"
#include "selectFile.h"
#include "sha1.h"

using namespace std;

class users
{
public:
	users();
	virtual ~users();
	string authenticateUser(const string & username, const string & password, const char *host);
	bool isValid(const char *id, const char *host);
	string getUsername(const string & id);
private:
	struct usr {
		string host;
		string name;
	};
	map<string, usr> clients;
	void addUser(const string & id, const string & host, const string & name);
	bool validateCouple(const string & user, const string & pass);
};

#endif /*USERS_H_*/
