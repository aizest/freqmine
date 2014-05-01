/*
 * ThreadJob.h
 *
 *  Created on: Apr 29, 2014
 *      Author: dazhang
 */

#ifndef THREADJOB_H_
#define THREADJOB_H_


typedef void* (*thread_func)(void* fdata);	//define the function pointer type

class ThreadJob {
public:
	thread_func func;	//function pointer
	void* fdata;
	void* fresult;
	//sem_t completed;	//info this job is completed

	ThreadJob(void* _fdata, void* _fresult, thread_func _func);
	~ThreadJob();
	//a deconstruction function may be needed, with free()

};


#endif /* THREADJOB_H_ */
