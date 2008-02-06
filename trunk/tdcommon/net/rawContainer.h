/** Teardrop - rawContainer.h

@brief This class handles raw data for getHttp

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 29/10/2006

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
