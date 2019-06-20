#include <pjlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "sync.h"
#include "job.h"
#include "testjob.h"
using namespace std;
int main()
{
	daemon(1,1);
	pid_t pid = fork();
	if(pid == 0)
	{
		testjob *jb = new testjob();
		jb->Execute();
		pj_thread_sleep(10);
		cout<<"child hello world "<<getpid()<<endl;
		jb->Stop();
		pj_thread_sleep(20*1000);
	}
	else
	{
		cout<<"hello world "<<getpid()<<endl;
		return 0;
	}
}
