#include "fp_thread_manager.h"

//A wrapper function, which will run the business job function.
//A pthread will be created for this function
void* ManageFunction(void* argv)
{
	FPThreadManager* pManager = (FPThreadManager*)argv;
	bool iSkip = false;
	int nWork;

	//the thread will be re-used until all the jobs are done
	while(true)
	{
		//The thread is blocked until being waken up
		pManager->waitSemaphore();
		if(pManager->isTerminated())
			break;

		printf("thread wakeup.\n");

		//Get a job from the job queue
		pManager->lockMutex();
		if(pManager->isEmpty())
		{
			printf("thread: no job to pop!\n");
			iSkip = true;
		}else
		{
			nWork = pManager->popJob();
		}
		pManager->unlockMutex();

		if(iSkip)//skip iteration if no job has been popped
		{
			iSkip = false;
		}else
		{
			printf("call the job function.\n");
			pManager->runJobFunction(nWork);
		}
	}

	printf("thread terminate.\n");
	return 0;
}


FPThreadManager::FPThreadManager(int (*threadFuction)(int), int nMaxThreadCnt) {

	sem_init(&m_sem, 0, 0);
	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_init(&t_mutex, NULL);
	threads_terminate = false;

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
	pthread_mutex_destroy(&t_mutex);
}

//check if thread pool is terminated
bool FPThreadManager::isTerminated()
{
	return threads_terminate;
}

//set termination flag
int FPThreadManager::setTerminate()
{
	this->lockTMutex();
	threads_terminate = true;
	this->unlockTMutex();

	//notify all threads that pool is terminated (we check flag threads_terminate right after wait_semaphore)
	list<FPThread*>::iterator it;
	for(it=m_threadList.begin(); it!=m_threadList.end();it++)
	{
		this->setSemaphore();
	}

	return 0;
}

//wait all threads to be terminated
int FPThreadManager::joinAllThreads()
{
	list<FPThread*>::iterator it;
	for(it=m_threadList.begin(); it!=m_threadList.end();it++)
	{
		(*it)->joinThread();
	}

	return 0;
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

// grab terminate flag lock
int FPThreadManager::lockTMutex()
{
	return pthread_mutex_lock(&t_mutex);
}

// release terminate flag lock
int FPThreadManager::unlockTMutex()
{
	return pthread_mutex_unlock(&t_mutex);
}

// Push job data into the queue
void FPThreadManager::pushJob(int jobData)
{
	m_jobQueue.push(jobData);
}

// check if the job queue is empty, return true if job queue is empty
bool FPThreadManager::isEmpty()
{
	return m_jobQueue.empty();
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
