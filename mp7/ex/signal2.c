/* In class exercise to install two signal handlers
 * Run in background, use "kill -10 PID" and "kill -12 PID" to test
 *
 * Notes  10 is SIGUSR1
 *        12 is SIGUSR2
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int Pos = 20;
int LastPos = 20;

void Left(int signum)
{
    LastPos = Pos;
    if ((Pos -= 5) < 0) Pos = 0;
}

void Right(int signum)
{
    LastPos = Pos;
    if ((Pos += 5) > 40) Pos = 40;
}

int main(void)
{
    int i;
    signal(SIGUSR1, Left);  /* install the handler */
    signal(SIGUSR2, Right); /* install the handler */

    while (1) {
        for (i=0; i<Pos; i++) printf(" ");

        if (LastPos > Pos) printf("<===\n");
        else if (LastPos < Pos) printf("===>\n");
        else printf("<==>\n");

        sleep(1);
    }
}

