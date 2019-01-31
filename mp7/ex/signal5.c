/* if set the ignore signal will not stop from terminal.  Must use kill
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
    void f(int);		/* prototype for signal handler function */

    signal(SIGINT, SIG_IGN);	/* install the handler */
    printf("You can't stop me now!\n");

    while (1) {
        printf("Nanny Nanny Boo Boo.\n");
        sleep(1);
    }
    return 0;
}

