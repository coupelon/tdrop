/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "UIDSession.h"

UIDSession::UIDSession(tdParam *tdp){
	this->tdp = tdp;
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

string UIDSession::addSearch(string userID, metaRank *mr, string userName) {
	string newID = getID();
	(*searches)[newID].mr = mr;
	(*searches)[newID].userID = userID;
	(*searches)[newID].userName = userName;
	return newID;
}

metaRank *UIDSession::getSearch(string userID, string searchID, string userName) {
	dumpToDisk();
	if (searches->find(searchID) != searches->end()) {
	  if ((*searches)[searchID].userID == userID)
		  return (*searches)[searchID].mr;
	}
	string file = selectFile::getHomeDirectory()
						+ HOME_SESSIONS
						+ userName
						+ "/"
						+ searchID
						+ ".xml";
	if (selectFile::fileExists(file)) {
		metaRank *mr = NULL;
		LOG4CXX_DEBUG(tdParam::logger, "Loading file back : " + file);
		if (openSave::xmlOpen(file, &mr, tdp)) {
			(*searches)[searchID].mr = mr;
			(*searches)[searchID].userID = userID;
			(*searches)[searchID].userName = userName;
			return mr;
		} else {
			LOG4CXX_INFO(tdParam::logger, "Loading failed.");
		}
	}
	return NULL;
}

void UIDSession::dumpToDisk() {
	list<map<string, clientSearch>::iterator> to_erase;
	for (map<string, clientSearch>::iterator srch = searches->begin();
		 srch != searches->end(); ++srch) {
		if (srch->second.mr->isFinished()) {
			string path = selectFile::getHomeDirectory()
						+ HOME_SESSIONS
						+ srch->second.userName;
			selectFile::createDirectory(path);
			LOG4CXX_DEBUG(tdParam::logger, "Dumping to disk : " + path+"/"+srch->first+".xml");
			openSave::xmlSave(path+"/"+srch->first+".xml",srch->second.mr);
			to_erase.push_back(srch);
		}
	}
	for(list<map<string, clientSearch>::iterator>::iterator ers = to_erase.begin();
		to_erase.end() != ers; ++ers) {
		delete (*ers)->second.mr;
		searches->erase(*ers);
	}
}
