#include <pthread.h>
#include "fp_thread.h"


FPThread::FPThread(void* (*threadFuction)(void*),void* threadArgv) {

	// Initiate the thread attribute
	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);

	pthread_create(&m_thread, &threadAttr, threadFuction, threadArgv);
}

FPThread::~FPThread() {
	// TODO Auto-generated destructor stub
}



void FPThread::joinThread()
{
	// call the join function of pthread
	pthread_join(m_thread, NULL);
}
