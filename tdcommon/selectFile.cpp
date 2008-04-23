/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#include "selectFile.h"

bool selectFile::find(string f, string e, string & path) {
    /*
    First check for the file in the current directory or with absolute path
    */
    if (fileexist(f + e)) { path=""; return true; }
    debug("couldn't find ./%s%s\n",f.c_str(),e.c_str());
    
    /*
    Then check the different home directory
    */
    string home = getHomeDirectory();
    if (fileexist(home + f + e)) { path = home; return true; }
    if (fileexist(home + "xml/" + f + e)) { path = home + "xml/"; return true; }
    if (fileexist(home + "icons/" + f + e)) { path = home + "icons/"; return true; }
    debug("couldn't find %s%s%s\n",home.c_str(),f.c_str(),e.c_str());
    
    /*
    If available, check the installation directory
    */
    #ifdef TEARDROP_DATADIR
        if (fileexist(TEARDROP_DATADIR + string("/") + f + e)) { path = TEARDROP_DATADIR + string("/"); return true; }
        debug("couldn't find %s/%s%s\n",TEARDROP_DATADIR,f.c_str(),e.c_str());
    #endif
        
    /*
    At last, check in the current directory, in the engine directory (for raw sources and Win32)
    */
    if (fileexist("engine/" + f + e)) { path = "engine/"; return true; }
    debug("couldn't find engine/%s%s\n",f.c_str(),e.c_str());
    
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
//    if ((h = getenv("HOME")) != NULL) {
//        return string(h) + HOME_LINUX;
//    }
    return HOME_LINUX;
}
