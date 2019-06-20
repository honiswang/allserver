#include "pjlib.h"
#include "sync.h"
#include "job.h"
#include "testjob.h"
#include <iostream>

testjob::testjob()
{
	SetName("testjob");
}

testjob::~testjob()
{
}

void testjob::Exec()
{
	std::cout<<"hello testjob"<<std::endl;
	pj_thread_sleep(1);
}
