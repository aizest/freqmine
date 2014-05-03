#ifndef FPTHREADMANAGER_H_
#define FPTHREADMANAGER_H_

#include <stdio.h>

#ifndef LIST_H_
#include <list>
#endif

#include <queue>
#include <semaphore.h>
#include <pthread.h>

#include "fp_thread.h"
#include "threadjob.h"

using namespace std;

typedef struct _threadPara{
	int tId;	//assigned thread id, start from 0 ~ ...
	void* tData;	//pointer to data package
	_threadPara(int _tId, void* _tData){
		tId = _tId;
		tData = _tData;
	}
}threadPara;

class FPThreadManager {
	friend void* ManageFunction(void*);
private:
	sem_t m_sem;	//Semaphore
	pthread_mutex_t m_mutex; // Mutex to protect the job queue
	pthread_mutex_t t_mutex; // protect threads_terminate

	bool threads_terminate;	//notify the termination of thread pools
	int m_nthreads;	//number of threads in thread size


	queue<ThreadJob*> m_jobQueue; // job queue
	queue<ThreadJob*> m_completeJobQueue;	//complete job queue, NOTE: it is user's duty to free() result* and data*
	list<sem_t*> jobSem;
	list<FPThread*> m_threadList; // thread list

public:
	FPThreadManager(int nMaxThreadCnt);
	virtual ~FPThreadManager();

	int joinAllThreads();

	bool isJobEmpty();

	bool isTerminated();

	int setTerminate();

	int waitSemaphore();

	int setSemaphore();

	int lockTMutex();

	int unlockTMutex();

	int lockMutex();

	int unlockMutex();

	void pushJob(ThreadJob* newJob);

	ThreadJob* popJob();

	int getSize();

	bool incrementSize(int incNum);

	void pushComJob(ThreadJob* comJob);

	bool cleanResult();

	bool isAllCompleted();

};

#endif
