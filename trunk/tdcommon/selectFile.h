/** 
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
#include "debugmacro.h"

#ifndef HOME_LINUX
#define HOME_LINUX "~/.teardrop/"
#endif

using namespace std;

class selectFile {
public:
    static bool find(string, string, string &);
    static string getHomeDirectory();
private:
    static bool fileexist(string);
};

#endif
