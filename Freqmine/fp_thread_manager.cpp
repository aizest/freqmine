#include <cstdlib>
#include "fp_thread_manager.h"

typedef struct _mrgfuncPara{
	void* tMrg;	//point to parent manager
	int t_Id;	//assigned id to the thread
	//FPThread* t_me;	//point of the thread

	_mrgfuncPara(void* _tMrg, int _t_Id){
		tMrg = _tMrg;
		t_Id = _t_Id;
		//t_me = NULL;
	}
}mrgfuncPara;

//A wrapper function, which will run the business job function.
//A pthread will be created for this function
void* ManageFunction(void* argv)
{
	mrgfuncPara* fpara = (mrgfuncPara*) argv;
	FPThreadManager* pManager = (FPThreadManager*)fpara->tMrg;
	//int threadId = *((int*) argv);
	threadPara* tPara = new threadPara(fpara->t_Id, NULL);
	ThreadJob* nJob;

	//the thread will be re-used until all the jobs are done
	while(true)
	{
		//The thread is blocked until being waken up
		pManager->waitSemaphore();
		if(pManager->isTerminated() && pManager->isJobEmpty())
			break;

		printf("thread wakeup.\n");

		//Get a job from the job queue
		pManager->lockMutex();
		if(pManager->isJobEmpty())
		{
			printf("thread: no job to pop!\n");
			continue;
		}else
		{
			nJob = pManager->popJob();
			pManager->pushComJob(nJob);
		}
		pManager->unlockMutex();


		//printf("call the job function.\n");
		//pManager->runJobFunction(nWork);
		tPara->tData = nJob->fdata;
		nJob->fresult = nJob->func((void*) tPara);	//run job
		//free(tPara->tData);	//is this safe????????????????????????????????

		tPara->tData = NULL;
		sem_post(nJob->sem_TJ);//set job complete flag

	}

	free(tPara);
	fpara->tMrg = NULL;
	free(fpara);

	printf("thread terminate.\n");
	return NULL;
}

//A wrapper function, which will run the business job function.
//A pthread will be created for this function
/*
void* ManageFunction(void* argv)
{
	FPThreadManager* pManager = (FPThreadManager*)argv;
	ThreadJob* nJob;

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
		if(pManager->isJobEmpty())
		{
			printf("thread: no job to pop!\n");
			continue;
		}else
		{
			nJob = pManager->popJob();
		}
		pManager->unlockMutex();


		printf("call the job function.\n");
		//pManager->runJobFunction(nWork);
		nJob->fresult = nJob->func(nJob->fdata);
		free(nJob);	//is this safe????????????????????????????????
		//terminate if job queue is empty (so far)
		if(pManager->isJobEmpty()){
			pManager->setTerminate();
		}
	}

	printf("thread terminate.\n");
	return NULL;
}*/


FPThreadManager::FPThreadManager(int nMaxThreadCnt)
{

	sem_init(&m_sem, 0, 0);
	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_init(&t_mutex, NULL);
	threads_terminate = false;

	//m_threadFuction = threadFuction;
	m_nthreads = nMaxThreadCnt;
	for(int i=0; i<m_nthreads; i++)
	{
		//Create CThread with the ManageFunction and the current CThreadManager
		mrgfuncPara* mfpara = new mrgfuncPara((void*)this, i);
		FPThread* pThread = new FPThread(ManageFunction, (void *)mfpara);
		printf("A new thread started.\n");
		m_threadList.push_back(pThread);
	}
}

FPThreadManager::~FPThreadManager()
{
	setTerminate();
	joinAllThreads();

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
	return pthread_mutex_lock(&m_mutex);
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
void FPThreadManager::pushJob(ThreadJob* newJob)
{
	newJob->sem_TJ = new sem_t();
	m_jobQueue.push(newJob);
	jobSem.push_back(newJob->sem_TJ);
	setSemaphore();
}

//push a job (from job queue) to complete job queue, a job is not completed until its sem_t being set
void FPThreadManager::pushComJob(ThreadJob* comJob)
{
	m_completeJobQueue.push(comJob);
}

// check if the job queue is empty, return true if job queue is empty
bool FPThreadManager::isJobEmpty()
{
	return m_jobQueue.empty();
}

// Pop a job data out of the job queue
ThreadJob* FPThreadManager::popJob()
{
	ThreadJob* nJob = m_jobQueue.front();
	m_jobQueue.pop();

	return nJob;
}

// Run the job function (specified by the constructor) with the specified job data
/*
int FPThreadManager::runJobFunction(int nWork)
{
	return (*m_threadFuction)(nWork);
}*/

//return size of threadpool, number of threads
int FPThreadManager::getSize()
{
	return m_nthreads;
}

//increase the size of threadpool
bool FPThreadManager::incrementSize(int incNum)
{
	return true;
}

//check whether all jobs have been completed
bool FPThreadManager::isAllCompleted()
{
	list<sem_t*>::iterator it;
	for(it = jobSem.begin(); it != jobSem.end(); it++)
	{
		//sem_t* tt = *it;
		sem_wait(*it);
	}
	return true;
}

//clean complete job queue (should I use lock?)
//remember to clean results, every time before use/reuse thread pool
bool FPThreadManager::cleanResult()
{
	if(!isJobEmpty())
	{
		return false;
	}

	ThreadJob* nJob;
	while(!m_completeJobQueue.empty())
	{
		nJob = m_completeJobQueue.front();
		m_completeJobQueue.pop();
		nJob->fdata = NULL;
		nJob->fresult = NULL;
		nJob->sem_TJ = NULL;
		free(nJob);
		nJob = NULL;
	}

	sem_t* nSem;
	while(!jobSem.empty())
	{
		nSem = jobSem.front();
		jobSem.pop_front();
		free(nSem);
		nSem = NULL;
	}

	return true;
}


