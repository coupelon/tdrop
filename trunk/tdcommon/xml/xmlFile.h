/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef XMLFILE_H
#define XMLFILE_H

#include <iostream>
#include <sstream>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace std;

class xmlFile {
public:
  xmlFile();
  ~xmlFile();

  /**
   * This method tries to open the given file
   * @return true in case of success
   */
  bool openFile(string);
  
  /**
   * Closes the file
   */
  void close();

  /**
   * Saves the XML doc to the specified file
   * @return true if saving succeeded
   */
  bool saveDocTo(string &);
  
  /** Convert an integer to a string */
  static string itoa(int);

  /** Convert a long to a string */
  static string ltoa(long);
  
  string & getFilename();
  xmlNode *getRootElement();
  
private:
  string filename;
  xmlDoc *doc;
  xmlNode *root_element;
};

#endif
