/*
 * ThreadJob.h
 *
 *  Created on: Apr 29, 2014
 *      Author: dazhang
 */

#ifndef THREADJOB_H_
#define THREADJOB_H_

#include <cstdlib>
#include <semaphore.h>

typedef void* (*thread_func)(void* fdata);	//define the function pointer type

class ThreadJob {
public:
	thread_func func;	//function pointer
	sem_t* sem_TJ;
	void* fdata;
	void* fresult;

	ThreadJob(void* _fdata, void* _fresult, thread_func _func);

};


#endif /* THREADJOB_H_ */
