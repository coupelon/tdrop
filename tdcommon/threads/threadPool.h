/** Teardrop - threadPool.h

@brief This file provides an interface to the boost tread library

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 30/08/2007

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

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <list>
#include <vector>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

class threadPool {
public:   
  threadPool(){
  }
          
  ~threadPool() {
  }

  void createThread(const function0<void>& pp){
    group.create_thread(pp);
  }
          
  void joinAll(){
    group.join_all();
  }
        
  recursive_mutex & getMutex() { return mtex; }
  
private:
  recursive_mutex mtex;
  thread_group group;
};

#endif
