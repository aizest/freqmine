
#ifndef FPTHREAD_H_
#define FPTHREAD_H_


class FPThread {
private:
	pthread_t m_thread;  //reference to the pthread
	//the following is for potential future use, each thread has its own queue, maybe job stealing?
	//pthread_mutex_t lock;
	//void* threadJQ;
public:
	FPThread(void* (*threadFuction)(void*),void* threadArgv);
	virtual ~FPThread();

	void joinThread();
};

#endif
