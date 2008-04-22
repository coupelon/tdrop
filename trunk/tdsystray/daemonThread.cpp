#include "daemonThread.h"

daemonThread::daemonThread(tdParam *tdp) {
	this->tdp = tdp;
}

daemonThread::~daemonThread() {
}

void daemonThread::run() {
	TdDaemon daemon;
    daemon.launchDaemon(tdp);
}
