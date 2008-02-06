/** Teardrop - selectFile.cpp

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
    char *h;
    string home;
    if ((h = getenv("HOME")) != NULL) {
        home = string(h) + "/";
        if (fileexist(home + ".teardrop/" + f + e)) { path = home + "/.teardrop/"; return true; }
        debug("couldn't find %s.teardrop/%s%s\n",home.c_str(),f.c_str(),e.c_str());
    } 
    /*Even if home was defined, under win32 it can still not be usable as the home
    directory. For instance, if you have cygwin installed, home is defined to the 
    cygwin's homes directory, instead of the ones in Document & Settings. So both
    directory need to be tested.
    */
    if ((h = getenv("APPDATA")) != NULL) {
        home = string(h) + "/";
        if (fileexist(home + "Teardrop/" + f + e)) { path = home + "Teardrop/"; return true; }
        debug("couldn't find %sTeardrop/%s%s\n",home.c_str(),f.c_str(),e.c_str());
    }
    
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
    if ((h = getenv("HOME")) != NULL) {
        return string(h) + "/.teardrop/";
    }
    return "";
}
