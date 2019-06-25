#include <pjlib.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sys/resource.h>
#include "sync.h"
#include "job.h"
#include "testjob.h"
#define CORE_SIZE   1024 * 1024 * 500
using namespace std;
int fd[2];
void handler(int signo)
{
	switch(signo)
	{
		case SIGABRT:
		case SIGHUP:
		case SIGQUIT:
		case SIGTERM:
			cout<<"get end signo"<<endl;
			write(fd[1],"end",strlen("end"));
			break;
		case SIGUSR2:
			printf("child: catch SIGUSR2\n");
			break;
		default:
			printf("should not be here\n");
			break;
	}
	return;
}

int main()
{
	daemon(1,1);
	if(pipe(fd) < 0)
	{
		cout<<"pipo error"<<endl;
		return 1;
	}

	pid_t pid = fork();
	pid_t pw;
	int status = 0;
	if(pid == 0)
	{
		struct rlimit rlmt;
		if (getrlimit(RLIMIT_CORE, &rlmt) == -1) {
			return -1; 
		}   
		rlmt.rlim_cur = (rlim_t)CORE_SIZE;
		rlmt.rlim_max  = (rlim_t)CORE_SIZE;

		if (setrlimit(RLIMIT_CORE, &rlmt) == -1) {
			return -1; 
		}   

		if (getrlimit(RLIMIT_CORE, &rlmt) == -1) {
			return -1; 
		} 
		pj_log_set_level(3);
		testjob *jb = new testjob(5000);
		jb->init();
		jb->Execute();
		cout<<"child hello world "<<getpid()<<endl;
		char buf[1024];
		while(1)
		{
			read(fd[0],buf,sizeof(buf));
			cout<<"get buf:"<<buf<<endl;
			if(strncmp(buf,"end",3) == 0)
			{
				break;
			}
		}
		Job::StopAll();
		return 0;
	}
	else
	{
		if(signal(SIGABRT,handler) ==SIG_ERR)
		{
			cout<<"add SIGABRT error"<<endl;
		}
		if(signal(SIGHUP,handler) ==SIG_ERR)
		{
			cout<<"add SIGHUP error"<<endl;
		}
		if(signal(SIGQUIT,handler) ==SIG_ERR)
		{
			cout<<"add SIGHUP error"<<endl;
		}
		if(signal(SIGTERM,handler) ==SIG_ERR)
		{
			cout<<"add SIGHUP error"<<endl;
		}
		pw = waitpid(pid,&status,0);
		cout<<"child hello world "<<pw<<" end status:"<<status<<endl;
		return 0;
	}
}
