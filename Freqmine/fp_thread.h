
#ifndef FPTHREAD_H_
#define FPTHREAD_H_


class FPThread {
private:
	pthread_t m_thread;  //reference to the pthread

public:
	FPThread(void* (*threadFuction)(void*),void* threadArgv);
	virtual ~FPThread();

	void joinThread();
};

#endif
