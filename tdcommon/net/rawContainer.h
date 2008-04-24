/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef RAWCONTAINER_H
#define RAWCONTAINER_H

/**
 * This class is designed to manage a consecutive memory space.
 */

class rawContainer {
public:
    rawContainer() {
        content = NULL;
        size = 0;
    }
    
    ~rawContainer() {
        if(content) free(content);
    }
    
    /**
     * Append data in the memory, reallocating memory as needed
     * @param pt A pointer to the data to copy
     * @param value The size of the data to copy in memory
     */
    void append(char *pt,size_t value) {
        content = (char *) realloc(content,(size+value)*sizeof(char *));
        memcpy(content+size,pt,value);
        size += value;
    }
    
    /**
     * @return A pointer to the allocated memory
     */
    char * getContent() { return content; }
    
    /**
     * @return The size of the allocated memory
     */
    size_t getLength() { return size; }
    
private:
    char *content;
    size_t size;
};

#endif
