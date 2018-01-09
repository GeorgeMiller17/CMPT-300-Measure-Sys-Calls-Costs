#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//the time for repeat
const int num = 10000;

//converting seconds into nano seconds
const int nanos = 1000000000;

//mesuring the minimal function call
struct timespec func_begin;
struct timespec func_stop;

//mesuring the minimal system call
struct timespec sys_begin;
struct timespec sys_stop;

//measuring  read or write system call,
struct timespec rwsys_begin;
struct timespec rwsys_stop;

void empty_func(){};// do nothing

//measuring of minimal function call
long long func_call()
{
	int i;
	long long sum_time=0;// total time
	long long aver_func_time=0;
	for(i=0; i<num;i++)
	{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&func_begin);
		empty_func();	//call a empty function call doing nothing
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &func_stop);
		//sum the time
		sum_time+=((func_stop.tv_sec * nanos) + func_stop.tv_nsec)-
		((func_begin.tv_sec * nanos) + func_begin.tv_nsec);
	}
		aver_func_time = sum_time / num;
		return aver_func_time;
}

//measuring of minimal system call
long long sys_call()
{
	int i;
	long long sum_time=0;// total time
	long long aver_sys_time=0;
	for(i=0; i<num;i++)
	{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sys_begin);
		getpid();//measuring the cost of getpid ()
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sys_stop);
		//sum the time
		sum_time+=((sys_stop.tv_sec * nanos) + sys_stop.tv_nsec)-
		((sys_begin.tv_sec * nanos) + sys_begin.tv_nsec);
	}
		aver_sys_time = sum_time / num;
		return aver_sys_time;
}


//measuring the cost of read/write
//fork a child process and transfer one byte between parent and child
long long rwsys_call()
{
	int i;
	int pipe_ret;
	pid_t pid;
	char a = 'a';
	long long sum_time=0;
	long long aver_rw_time=0;
	for (i=0;i<num; i++)
	{
		int pipefd[2];//set up a pipe for parent and child process
		pipe_ret = pipe(pipefd);
		pid = fork();//fork a child process
		if(pid>0){//parent process
			close(pipefd[0]);//parent process writes only
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&rwsys_begin);
			write(pipefd[1], &a, sizeof(a));
			wait(NULL);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &rwsys_stop);
			close(pipefd[1]);//terminate the pipe
		}
		else{//child process reads only
			close(pipefd[1]);
			read(pipefd[0], &a, sizeof(a));
			exit(0);}
			sum_time+=((rwsys_stop.tv_sec * nanos) + rwsys_stop.tv_nsec)-
			((rwsys_begin.tv_sec * nanos) + rwsys_begin.tv_nsec);
		}
			aver_rw_time = sum_time / num;
			return aver_rw_time;
}

int main(void){
	long long  aver_sys_time, aver_func_time,aver_rw_time;
	printf("The program repeats the experiment %d times and then take the average cost(nanoseconds)\n", num);
	aver_func_time = func_call();//calling the minimal function call
	printf("Task 1: Measure the cost of a minimal function call: %llu\n\n", aver_func_time);
	aver_sys_time = sys_call();//calling the minimal system call function
	printf("Task 2: Measure the cost of a minimal system call: %llu\n\n", aver_sys_time);
	aver_rw_time = rwsys_call();//calling the read/write system call function
	printf("Task 3: Measure the cost of a read/write system call: %llu\n\n", aver_rw_time);
	return 0;
}
