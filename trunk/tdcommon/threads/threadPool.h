/*
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <list>
#include <vector>
#include <pthread.h>

using namespace std;

/**
 * This class is a Thread pool. It can manage the creation and
 * work of a given amount of threads, making sure that at most
 * a certain number of threads are executed at the same time.
 */
template <typename T>
class threadPool {
    public:
        /**
         * This structure holds the parameters that a thread can be passed
         */
        struct threadArgs {
            void *methodToLaunch;
            bool isFinished;
            pthread_t *tdPointer;
            threadPool<T> *pParent;
        };
        
        /**
         * Creates a new threadPool.
         * @param detached If true the threads are created detached, meaning that they can't be joined
         * @param mt The maximum number of threads
         */
        threadPool(int mt, bool detached = false){
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
        
        /**
         * Creates a new thread in the pool. That thread must be a function
         * or seem to be, so if we give an object in argument, it must implement
         * the operator()(). 
         */
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
        
        /*
         * This should be called only when every wanted thread
         * were added to the pool
         */
        bool isAlive() {
            if (tpWaitingLine.size() > 0) return true;
            for(int i = 0; i < tpThreadList.size(); ++i) {
                if (!tpThreadList[i]->isFinished) return true;
            }
            return false;
        }
        
        /**
         * This method returns the number of threads that
         * have already returned. 
         */
        int howManyFinished() {
            int count = 0;
            for(int i = 0; i < tpThreadList.size(); ++i) {
                if (tpThreadList[i]->isFinished) count++;
            }
            return count;
        }
        
        /**
         * Joins every pending threads. When returning,
         * every threads should be finished
         */
        void joinAll(){
            while (tpWaitingLine.size() > 0) sched_yield();
            for(unsigned int i = 0; i < tpThreadList.size(); ++i) {
                pthread_join(*tpThreadList[i]->tdPointer, NULL);
            }
            tpNbThread = 0;
        }        
        
        /**
         * Lock the threadPool mutex given in parameter
         */
        static void lock(threadPool *tp){
            pthread_mutex_lock( &tp->mtex );
        }
        
        /**
         * Lock the threadPool mutex given in parameter
         */
        static void lock(threadPool & tp){
            pthread_mutex_lock( &tp.mtex );
        }
        
        /**
         * Unlock the threadPool mutex given in parameter
         */
        static void unlock(threadPool *tp){
            pthread_mutex_unlock( &tp->mtex );
        }
        
        /**
         * Unlock the threadPool mutex given in parameter
         */
        static void unlock(threadPool & tp){
            pthread_mutex_unlock( &tp.mtex );
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
