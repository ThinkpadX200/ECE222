
/* Demonstrate random number generation. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main ()
{
    time_t	clock;
    int	guessme,guess;

    clock=time(NULL);
    srand(clock);

    guessme=rand()%10+1;

    while (1) {
        printf("Guess? ");
        scanf("%d",&guess);
        if (guess == guessme) {
            printf("Correct!\n");
            exit(0);
        } else if (guess < guessme)
            printf("Higher\n");
        else if (guess > guessme)
            printf("Lower\n");
    }
    return 0;
}

