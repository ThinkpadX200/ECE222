/* Sending a signal using the kill() function (pgs 233-234)
 *
 * Shows parent can still collect input from stdin.  However,
 * this design cannot also use the wait() command to determine if 
 * the child quits because wait blocks.
 *
 * Note parent and child are both connected to stdin and stdout
 *
 * Note global variable to allow signal handler to communicate with
 * process that installed the handler.
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int running;

int main()
{
    void f(int);
    int i;
    char text[80];

    i=fork();	    /* parent & child process running now */
    if (i == 0) {
        /* child process */
        signal(SIGUSR1,f);  /* install the handler */
        printf("Child waiting...\n");
        running=1;
        while (running == 1)
            sleep(2);
        printf("Child now ready to quit...\n");
    } else {
        printf("please frog or quit\n");
        while (1) {
            sleep(1);  // give child chance to print first
            printf("Command? ");
            scanf("%s",text);
            if (strcmp(text,"frog") == 0)
                kill(i,SIGUSR1);
            if (strcmp(text,"quit") == 0)
                break;
        }
    }
    return 0;
}

void f(int signum)
{
    printf("Child received a frog!\n");
    running=0;
}

