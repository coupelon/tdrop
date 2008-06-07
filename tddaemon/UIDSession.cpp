/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "UIDSession.h"

UIDSession::UIDSession(){
	//Initialize le random generator on startup
	initRand();
	
	searches = new map<string, clientSearch>();
}

UIDSession::~UIDSession(){
	delete searches;
}

string UIDSession::getID() {
	const char *alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	string id ="";
  for(int index=0; index<29; index++){
  	char c = alphanum[int(rand()/(RAND_MAX + 1.0)*62)];
    id += c; 
  }
  return id;
}

void UIDSession::initRand() {
	srand((unsigned)time(0));
}

string UIDSession::addSearch(string userID, metaRank *mr) {
	string newID = getID();
	(*searches)[newID].mr = mr;
	(*searches)[newID].userID = userID;
	return newID;
}

metaRank *UIDSession::getSearch(string userID, string searchID) {
	if (searches->find(searchID) != searches->end()) {
	  if ((*searches)[searchID].userID == userID)
		  return (*searches)[searchID].mr;
	}
	return NULL;
}
