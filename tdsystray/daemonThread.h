/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef DAEMONTHREAD_H_
#define DAEMONTHREAD_H_

#include <QThread>
#include "tddaemon.h"

class daemonThread : public QThread
{
	Q_OBJECT
public:
	daemonThread(tdParam *tdp);
	virtual ~daemonThread();
	
protected:
    void run();
    
private:
    tdParam *tdp;
};

#endif /*DAEMONTHREAD_H_*/
