#ifndef FPTHREADMANAGER_H_
#define FPHREADMANAGER_H_

#include <stdio.h>

#ifndef LIST_H_
#include <list>
#endif

#include <queue>
#include <semaphore.h>

#include "fp_thread.h"
//#include "fp_tree.h"

using namespace std;

class FP_tree;

class FPThreadManager {
	friend void* ManageFunction(void*);
private:
	sem_t m_sem;	//Semaphore
	pthread_mutex_t m_mutex; // Mutex to protect the job queue
	pthread_mutex_t t_mutex; // protect threads_terminate

	void* data; //a pointer to the struct which stores the parameters of the function
	FP_tree *tree;

	bool threads_terminate;	//notify the termination of thread pools

	queue<int> m_jobQueue; // job queue
	list<FPThread*> m_threadList; // thread list

	int (*m_threadFuction)(FP_tree*, int); //pointer of job function


public:
	FPThreadManager(int (*threadFuction)(FP_tree*, int), int nMaxThreadCnt);
	FPThreadManager(int (*threadFuction)(FP_tree*, int), int nMaxThreadCnt, void* data);
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

	void pushJob(int nWork);

	int popJob();

	int runJobFunction(int nWork);

	void setFunction(int (*threadFuction)(FP_tree*, int));

	void setParameter(void* pData);

	void setCounter(int count);

	void setFPTree(FP_tree* fpTree);
};

#endif
