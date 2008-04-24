/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef TDEXCP_H
#define TDEXCP_H

#include <exception>
using namespace std;

/**
 * This class is here for exceptions handling in Teardrop
 */

class tdExcp: public exception {
public:
  tdExcp(string s) : exception() {
      error = s;
  }
  ~tdExcp() throw() {};
  virtual const char* what() const throw ()
  {
    return error.c_str();
  }
private:
  string error;
};

#endif
