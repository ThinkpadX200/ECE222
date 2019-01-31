/*
 *	Fork the program into parent & child processes.  Install a signal
 *	handler in each for one of the SIGUSR signals.  Have each send
 *	(using kill) a signal to the other three times, then exit.  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int Running;
int GotSigNum;

int main()
{
    void f1(int),f2(int);
    int child_pid;
    int	parent_pid;

    parent_pid=getpid();
    printf("Parents PID is %d\n",parent_pid);
    child_pid=fork();	    // parent & child process Running now
    if (child_pid == 0) {
        // child process 
        printf("Child finds own PID is %d\n", getpid());
        signal(SIGUSR1, f1); /* install the handler */
        while (Running < 3) {
            pause();
            printf("Child received SIGUSR1 %d. count=%d\n", GotSigNum, Running);
            sleep(1);
            kill(parent_pid, SIGUSR2);  // parent pid saved before fork
        }
    } else {
        signal(SIGUSR2, f2); /* install the handler */
        Running = 100;
        while (Running < 105) {
            sleep(2);
            kill(child_pid, SIGUSR1);   // learned child pid from fork
            pause();
            printf("Parent received SIGUSR2 %d. count=%d\n", GotSigNum, Running);
        }
    }
    return 0;
}

void f1(int signum)
{
    Running++;
    GotSigNum = signum;
}

void f2(int signum)
{
    Running += 2;
    GotSigNum = signum;
}

