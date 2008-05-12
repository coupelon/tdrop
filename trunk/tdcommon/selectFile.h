/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef SELECTFILE_H
#define SELECTFILE_H

#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include "debugmacro.h"

#ifndef HOME_LINUX
#define HOME_LINUX "/.teardrop/"
#endif

#define CONFIG_FILE "config.xml"

using namespace std;

class selectFile {
public:
		/**
		 * This method searches for a file on the known directories
		 * @param f the file name
		 * @param path if the file is found, this path will be set to the actual file path + name
		 * @return True is the file was found
		 */
    static bool find(string f, string & path);
    
    /**
     * This method is used to get the Teardrop's home configuration directory
     * @return the path to the Teardrop's directory
     */
    static string getHomeDirectory();
    
    /**
     * This method returns the filename of the given string, either it
     * being a windows path, linux path or a web url.
     * @param str the string containing the path+filename
     * @return the file name
     */
    static string getFilename(const string& str);
    
    /**
     * This method returns the filename of the given string, 
     * without its extension.
     * @param str the string containing the filename
     * @return the base file name
     */
    static string getBasename(const string& str);
    
    /**
     * @return The full path of the personnal config file.
     */
    static string getHomeConfigFile();
    
    /**
     * Creates the home directory structure
     */
     static void createDirectoryStructure();
private:
    static bool fileexist(string);
};

#endif
