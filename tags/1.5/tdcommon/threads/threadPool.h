/** Teardrop - threadPool.h

@brief This file provides an interface to the pthread library

@author Olivier COUPELON (olivier.coupelon@wanadoo.fr)
@date 27/10/2006

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
#include <pthread.h>

using namespace std;

#define THMAX 16

template <typename T>
class threadPool {
    public:
        
        struct threadArgs {
            void *methodToLaunch;
            bool isFinished;
            pthread_t *tdPointer;
            threadPool<T> *pParent;
        };
        
        threadPool(bool detached = false, int mt = THMAX){
            tpMaxThread = mt;
            tpNbThread = 0;
            pthread_mutex_init(&mtex,NULL);
            pthread_attr_init(&attr);
            if (detached) pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        }
        
        threadPool(threadPool & tp){
            tpMaxThread = tp.tpMaxThread;
            tpThreadList = tp.tpThreadList;
            tpNbThread = tp.tpNbThread;
            mtex = tp.mtex;
        }
        
        ~threadPool() {
            for(unsigned int i = 0; i < tpThreadList.size(); ++i) {
                delete tpThreadList[i]->tdPointer;
                delete (T *) (tpThreadList[i]->methodToLaunch);
                delete tpThreadList[i];
            }
        }
        
        void createThread(void * pp){
            pthread_t *newThread = new pthread_t();
            
            threadArgs *newArg = new threadArgs;
            newArg->tdPointer = newThread;
            newArg->methodToLaunch = pp;
            newArg->pParent = this;
            newArg->isFinished = false;
            threadPool::lock(this);
            if (tpNbThread < tpMaxThread) {
                tpThreadList.push_back(newArg);
                tpNbThread++;
                pthread_create(newThread,&attr,launchThread, (void *) newArg);
            } else {
                tpWaitingLine.push_back(newArg);
            }
            threadPool::unlock(this);
        }
        
        /* Should be called only when every thread that should have been launched are */
        bool isAlive() {
            if (tpWaitingLine.size() > 0) return true;
            for(int i = 0; i < tpThreadList.size(); ++i) {
                if (!tpThreadList[i]->isFinished) return true;
            }
            return false;
        }
        
        int howManyFinished() {
            int count = 0;
            for(int i = 0; i < tpThreadList.size(); ++i) {
                if (tpThreadList[i]->isFinished) count++;
            }
            return count;
        }
        
        void joinAll(){
            while (tpWaitingLine.size() > 0) sched_yield();
            for(unsigned int i = 0; i < tpThreadList.size(); ++i) {
                pthread_join(*tpThreadList[i]->tdPointer, NULL);
            }
            tpNbThread = 0;
        }
        
        void cancelAll() {
            for(int i = 0; i < tpThreadList.size(); ++i) {
                pthread_cancel(*tpThreadList[i]->tdPointer);
                tpThreadList[i]->isFinished = true;
            }
            tpNbThread = 0;
        }
        
        static void lock(threadPool *tp){
            pthread_mutex_lock( &tp->mtex );
        }
        
        static void lock(threadPool & tp){
            pthread_mutex_lock( &tp.mtex );
        }
        
        static void unlock(threadPool *tp){
            pthread_mutex_unlock( &tp->mtex );
        }
        
        static void unlock(threadPool & tp){
            pthread_mutex_unlock( &tp.mtex );
        }
        
        void setMaxThread(int mt = THMAX) {
            tpMaxThread = mt;
        }
        
    private:
        int tpMaxThread;
        vector<threadArgs *> tpThreadList;
        list<threadArgs *> tpWaitingLine;
        int tpNbThread;
        pthread_mutex_t mtex;
        pthread_attr_t attr;
    
        static void * launchThread (void * t) {
            threadArgs *ta = (threadArgs *) t;
            (*(T *) ta->methodToLaunch)();
            ta->isFinished = true;
            ta->pParent->injectNextThread();
            return NULL;
        }
        
        void injectNextThread() {
            threadPool::lock(this);
            if (tpWaitingLine.size() > 0) {
                threadArgs *args = tpWaitingLine.front();
                tpThreadList.push_back(args);
                pthread_create(args->tdPointer,&attr,launchThread, (void *) args);
                tpWaitingLine.pop_front();
            } else  {
                //This method is only called by launchThread which has not yet decreased tpNbThread
                tpNbThread--;
            }
            threadPool::unlock(this);
        }
};

#endif
