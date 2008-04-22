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
