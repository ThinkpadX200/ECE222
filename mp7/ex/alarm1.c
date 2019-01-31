/* Example of alarm with an alarm handler.
 *
 * Like sleep but
 *     allows handler to be called and
 *     does not block waiting for alarm to ring
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

typedef void (*fptr)();

void CtrlCHandler(int signum)
{
    static int i;
    if (++i > 5) {
        exit(0);
    }
    printf("CTRL-C Sent.\n");
}

void AlarmHandler(int signum)
{
    static int i;
    if (++i > 20) {
        exit(0);
    }
    printf("Alarm Signalled!!!.\n");
    alarm(rand()%5 + 1);
}

int main(void)
{
    signal(SIGINT, (fptr)CtrlCHandler);	/* install the handler */
    signal(SIGALRM, (fptr)AlarmHandler);
    alarm(1);

    while (1) {
        printf("Waiting...\n");
        sleep(1);
    }
}

