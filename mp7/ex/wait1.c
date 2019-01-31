/* example using wait() to learn when child process ends
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define TIRED 5

void DoWhatChildrenDo(void)
{
    int i;
    for (i=0; i<TIRED; i++) {
        printf("\tI won't eat my spinach!!!\n");
        sleep(1);
    }
    printf("\tO.k.  I'll eat my spinach...\n");
    exit(2);
}

void DoWhatParentsDo(int pid)
{
    int wait_ret;
    wait_ret = wait(NULL);
    printf("Waited for %d.  Return value = %d\n", pid, wait_ret);
    printf("Here's your Ice Cream.\n");
}

int main(void)
{
    int pid;
    pid = fork();
    switch (pid) {
    case 0: // Child
        DoWhatChildrenDo();
        break;

    default:
        DoWhatParentsDo(pid);
        break;
    }
    return 0;
}
