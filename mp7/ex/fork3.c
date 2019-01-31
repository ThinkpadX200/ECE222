#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int i, j;

    i = fork();

    for (j=0; j<10; j++) {
        if (i == 0) {	/* child process */
            printf("\t\t\tI am a child\n");
            sleep(2);
        } else {	/* parent process */
            printf("This is the parent\n");
            sleep(1);
        }
    }
    return 0;
}
