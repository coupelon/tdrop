/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef VALIDPAGES_H
#define VALIDPAGES_H

#include <iostream>
#include "net/getHttp.h"
#include "pthread.h"

using namespace std;

class validPages {
public:
	/**
	 * @param u The url to check
	 * @param r A boolean pointer which will be set according to the url validity
	 * @param t The Teardrop's parameters
	 */
  validPages(string u,bool *r, tdParam *t);
  
  /**
   * Starts the validation
   */
  void validate();
  
  /**
   * A convenience method to allow for threads usage
   */
	void operator()() { validate(); }
	
private:
  string url;
  bool *results;
  tdParam *tdp;
};

#endif
