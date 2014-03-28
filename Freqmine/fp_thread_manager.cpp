#include "fp_thread_manager.h"

//A wrapper function, which will run the business job function.
//A pthread will be created for this function
void* ManageFunction(void* argv)
{
	FPThreadManager* pManager = (FPThreadManager*)argv;

	//the thread will be re-used until all the jobs are done
	while(true)
	{
		//The thread is blocked until being waken up
		pManager->waitSemaphore();
		printf("thread wakeup.\n");

		//Get a job from the job queue
		pManager->lockMutex();
		int nWork = pManager->popJob();
		pManager->unlockMutex();

		printf("call the job function.\n");
		pManager->runJobFunction(nWork);
	}

	return 0;
}


FPThreadManager::FPThreadManager(void (*threadFuction)(int), int nMaxThreadCnt) {

	sem_init(&m_sem, 0, 0);
	pthread_mutex_init(&m_mutex, NULL);

	m_threadFuction = threadFuction;

	for(int i=0; i<nMaxThreadCnt; i++)
	{
		//Create CThread with the ManageFunction and the current CThreadManager
		FPThread* pThread = new FPThread(ManageFunction, this);
		printf("A new thread started.\n");
		m_threadList.push_back(pThread);
	}
}

FPThreadManager::~FPThreadManager()
{
	sem_destroy(&m_sem);
	pthread_mutex_destroy(&m_mutex);

	list<FPThread*>::iterator it;
	for(it=m_threadList.begin(); it!=m_threadList.end();it++)
	{
		(*it)->joinThread();
	}
}

// Semaphore waits
int FPThreadManager::waitSemaphore()
{
	return sem_wait(&m_sem);
}

// Set semaphore
int FPThreadManager::setSemaphore()
{
	return sem_post(&m_sem);
}

// Grab the lock
int FPThreadManager::lockMutex()
{
	int n= pthread_mutex_lock(&m_mutex);
	return n;
}

// Release the lock
int FPThreadManager::unlockMutex()
{
	return pthread_mutex_unlock(&m_mutex);
}

// Push job data into the queue
void FPThreadManager::pushJob(int jobData)
{
	m_jobQueue.push(jobData);
}

// Pop a job data out of the job queue
int FPThreadManager::popJob()
{
	int nWork = m_jobQueue.front();
	m_jobQueue.pop();

	return nWork;
}

// Run the job function (specified by the constructor) with the specified job data
int FPThreadManager::runJobFunction(int nWork)
{
	return (*m_threadFuction)(nWork);
}
