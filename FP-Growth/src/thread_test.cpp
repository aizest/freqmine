#include <stdio.h>
#include <unistd.h>

#include "fp_thread_manager.h"

using namespace std;

// A sample job function, which calculates the square of the specified integer
int square(int nWork)
{
	int nResult = nWork * nWork;
	printf("Processing work[%d], calculation result is %d\n", nWork, nResult);

	return 0;
}

/*int main() {

	// Create an instance of FPThreadManager by passing the job function and the number of threads
	FPThreadManager* pManager = new FPThreadManager(square, 3);

	// Push the job data into the job queue
	pManager->pushJob(5);
	pManager->pushJob(20);
	pManager->pushJob(30);
	pManager->pushJob(10);
	pManager->pushJob(40);

	// Wake up the threads by setting the semaphore
	pManager->setSemaphore();
	pManager->setSemaphore();
	pManager->setSemaphore();
	pManager->setSemaphore();
	pManager->setSemaphore();

	// Wait for the execution of pthreads
	sleep(1);

	return 0;
}*/
