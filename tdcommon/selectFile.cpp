/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "selectFile.h"

bool selectFile::find(string f, string & path) {
    /*
    First check for the file in the current directory or with absolute path
    */    
    if (fileExists(f)) { path=f; return true; }
    LOG4CXX_DEBUG(tdParam::logger,"couldn't find ./" + f);
    
    /*
    Then check the different home directory
    */
    string home = getHomeDirectory();
    if (fileExists(home + f)) { path = home + f; return true; }
    if (fileExists(home + "xml/" + f)) { path = home + "xml/" + f; return true; }
    if (fileExists(home + "icons/" + f)) { path = home + "icons/" + f; return true; }
    LOG4CXX_DEBUG(tdParam::logger,"couldn't find " + home + "/{.|xml|icons}/ " + f);
    
    /*
    If available, check the installation directory
    */
    #ifdef TEARDROP_DATADIR
        if (fileExists(TEARDROP_DATADIR + string("/") + f)) { path = TEARDROP_DATADIR + string("/") + f; return true; }
        LOG4CXX_DEBUG(tdParam::logger,"couldn't find " << TEARDROP_DATADIR << "/" << f);
    #endif
    
    LOG4CXX_WARN(tdParam::logger,"File " + f + " not found.");
    path = "";
    return false;
}

bool selectFile::fileExists(string f) {
    fstream fin;
    fin.open(f.c_str(),ios::in);
    bool flag = fin.is_open();
    fin.close();
    return flag;
}

string selectFile::getHomeDirectory() {
    char *h;
    if ((h = getenv("APPDATA")) != NULL) {
        return string(h) + "/Teardrop/";
    }
    if ((h = getenv("HOME")) != NULL) {
        return string(h) + HOME_LINUX;
    }
    return HOME_LINUX;
}

string selectFile::getFilename(const string& str) {
  size_t found;
  found=str.find_last_of("/\\");
  return str.substr(found+1);
}

string selectFile::getBasename(const string& str) {
	return str.substr(0,str.find_last_of("."));
}

string selectFile::getHomeConfigFile() {
	return getHomeDirectory() + CONFIG_FILE;
}

void selectFile::createDirectoryStructure() {
		/* Create the Teardrop personal directory if it doesn't exist */
    string path = selectFile::getHomeDirectory();
 
    createDirectory(path);
    createDirectory(path+HOME_XML);
    createDirectory(path+HOME_ICONS);
    createDirectory(path+HOME_SESSIONS);
    if (!fileExists(getHomeConfigFile())) {
    	ofstream config;
			config.open(selectFile::getHomeConfigFile().c_str(), ios::out | ios::app | ios::binary);
			config << "<?xml version=\"1.0\"?>"
						 << endl << "<config>" << endl
						 << "</config>" << endl;
			config.close();
    }
}

bool selectFile::createDirectory(string path) {
	umask(0);
	#ifdef WIN32
	return !mkdir(path.c_str());
  #else
  return !mkdir(path.c_str(),0700);
  #endif
}

