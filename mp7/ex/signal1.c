/* Example handing a signal.  This is SIGINT -- interrupt from keyboard with
 * Control-C.  Or use kill -s 2 PID#
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void SignalHandler(int signum)
{
    static int i;
    switch (i++) {
    case 0:
        printf("Yes.\n");
        break;
    case 1:
        printf("YES!\n");
        break;
    case 2:
        printf("YES!!!\n");
        break;
    case 3:
        printf("YES! YES!! YES!!!!\n");
        break;
    case 4:
        printf("SHUT UP!!!\n");
        break;
    case 5:
        printf("BANG!\n");
        break;
    case 6:
        printf("BANG! BANG! BANGBANGBANGBANGBANG!!!!!\n");
        exit(0);
    }
}

int main(void)
{
    signal(SIGINT, SignalHandler);	/* install the handler */

    while (1) {
        printf("Can you hear me now?\n");
        sleep(1);
    }
    return 0;
}

