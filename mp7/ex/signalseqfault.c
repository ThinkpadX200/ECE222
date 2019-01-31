#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

typedef void (*fptr)();

void SignalHandler(int signum)
{
    static int i;

    switch (i++) {
    case 0:
        printf("Shouldn't have done that.\n");
        break;
    case 1:
        printf("Don't do that again.\n");
        break;
    case 2:
        printf("I said STOP it!\n");
        break;
    case 3:
        printf("One more time and you're through!\n");
        break;
    case 4:
        printf("That's my last warning!\n");
        signal(SIGSEGV, SIG_DFL);
        break;
    case 5:
        printf("Three strikes and you're out!\n");
        exit(0);
    }
}

int main(void)
{

    signal(SIGSEGV, (fptr)SignalHandler);	/* install the handler */

    while (1) {
        printf("*0 = 0;\n");
        *(char *)0 = 0;
        printf("Ooops, Sorry...");
        fflush(stdout);
        sleep(1);
    }
    return 0;
}

