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
    if (fileexist(f)) { path=f; return true; }
    debug("couldn't find ./%s\n",f.c_str());
    
    /*
    Then check the different home directory
    */
    string home = getHomeDirectory();
    if (fileexist(home + f)) { path = home + f; return true; }
    if (fileexist(home + "xml/" + f)) { path = home + "xml/" + f; return true; }
    if (fileexist(home + "icons/" + f)) { path = home + "icons/" + f; return true; }
    debug("couldn't find %s/{.|xml|icons}/%s\n",home.c_str(),f.c_str());
    
    /*
    If available, check the installation directory
    */
    #ifdef TEARDROP_DATADIR
        if (fileexist(TEARDROP_DATADIR + string("/") + f)) { path = TEARDROP_DATADIR + string("/") + f; return true; }
        debug("couldn't find %s/%s\n",TEARDROP_DATADIR,f.c_str());
    #endif
    
    path = "";
    return false;
}

bool selectFile::fileexist(string f) {
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

