/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef RAWCONTAINER_H
#define RAWCONTAINER_H

class rawContainer {
public:
    rawContainer() {
        content = NULL;
        size = 0;
    }
    
    ~rawContainer() {
        if(content) free(content);
    }
    
    void append(char *pt,size_t value) {
        content = (char *) realloc(content,(size+value)*sizeof(char *));
        memcpy(content+size,pt,value);
        size += value;
    }
    
    char * getContent() { return content; }
    size_t getLength() { return size; }
    
private:
    char *content;
    size_t size;
};

#endif
