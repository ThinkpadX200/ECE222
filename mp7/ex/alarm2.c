/* Example of alarm with an alarm handler.
 *
 * Like sleep but
 *     allows handler to be called and
 *     does not block waiting for alarm to ring
 *
 * printf() calls should not be in signal handlers.  Instead
 * set a flag to indicate the signal was called and clear it
 * after after the handler returns
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int CtrlCCaught = 0;
int AlarmRang = 0;

void CtrlCHandler(int signum)
{
    static int i;
    if (++i > 5) {
        exit(0);
    }
    CtrlCCaught = 1;
}

void AlarmHandler(int signum)
{
    static int i;
    if (++i > 20) {
        exit(0);
    }
    AlarmRang = 1;
}

int main(void)
{
    signal(SIGINT, CtrlCHandler);	/* install the handler */
    signal(SIGALRM, AlarmHandler);
    int foundsignal;
    alarm(1);

    while (1) {
        foundsignal = 0;
        printf("Paused waiting for a signal...\n");
        pause();
        if (CtrlCCaught == 1) {
            CtrlCCaught = 0;
            foundsignal++;
            printf("CTRL-C Sent.\n");
        } 
        if (AlarmRang > 0) {
            AlarmRang = 0;
            int snooze = rand()%5 + 1;
            alarm(snooze);
            printf("Alarm Signalled. Next in %d seconds.\n", snooze);
            foundsignal++;
        }
        // sanity checking
        if (foundsignal == 0) {
            printf("What happened?  pause returns but what signal?\n");
        } else if (foundsignal > 1) {
            printf("What happened?  Two interrupts between calls to pause?\n");
        }
    }
}

