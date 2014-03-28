#ifndef CTHREADMANAGER_H_
#define CTHREADMANAGER_H_

#include <stdio.h>
#include <list>
#include <queue>
#include <semaphore.h>

#include "fp_thread.h"

using namespace std;

class FPThreadManager {
	friend void* ManageFunction(void*);
private:
	sem_t m_sem;	//Semaphore
	pthread_mutex_t m_mutex; // Mutex

	queue<int> m_jobQueue; // job queue
	list<FPThread*> m_threadList; // thread list

	int (*m_threadFuction)(int); //pointer of job function


public:
	FPThreadManager(int (*threadFuction)(int), int nMaxThreadCnt);
	virtual ~FPThreadManager();

	int waitSemaphore();

	int setSemaphore();

	int lockMutex();

	int unlockMutex();

	void pushJob(int nWork);

	int popJob();

	int runJobFunction(int nWork);
};

#endif /* CTHREADMANAGER_H_ */
