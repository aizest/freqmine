/*
 * ThreadJob.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: dazhang
 */
#include "threadjob.h"

ThreadJob::ThreadJob(void* _fdata, void* _fresult, thread_func _func)
{
	fdata = _fdata;
	fresult = _fresult;
	func = _func;
}


