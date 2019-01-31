/* Example forking and waiting for three child processes and learning
 * which child precess ended.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROCESSES 10
void DoFirstChild(void)
{
    int i;
    for (i=0; i<5; i++) {
        printf("\t2: First child Medium...\n");
        sleep(2);
    }
}

void DoSecondChild(void)
{
    int i;
    for (i=0; i<5; i++) {
        printf("\t\t3: Second child Slow...\n");
        sleep(3);
    }
}

void DoThirdChild(void)
{
    int i;
    for (i=0; i<5; i++) {
        printf("\t\t\t1: Third child Fast...\n");
        sleep(1);
    }
}

int which_process(int wait_ret, int child_pids[], int processes)
{
    int i;
    for (i = 0; i < processes; i++)
        if (child_pids[i] == wait_ret)
            return i;
    return -1;
}

int main(void)
{
    int wait_ret;
    int process = 0;
    int child_pids[MAX_PROCESSES];

    child_pids[process] = fork();
    if (child_pids[process] != 0) {
        // parent process
        process++;
        child_pids[process] = fork();
        if (child_pids[process] != 0) {
            // parent process
            process++;
            child_pids[process] = fork();
            if (child_pids[process] != 0) {
                // parent process
                process++;
                wait_ret = wait(NULL);
                printf("Waited for %d (child %d) to finish.\n", wait_ret,
                       which_process(wait_ret, child_pids, process));

                wait_ret = wait(NULL);
                printf("Waited for %d (child %d) to finish.\n", wait_ret,
                       which_process(wait_ret, child_pids, process));

                wait_ret = wait(NULL);
                printf("Waited for %d (child %d) to finish.\n", wait_ret,
                       which_process(wait_ret, child_pids, process));
            } else DoThirdChild();
        } else DoSecondChild();
    } else DoFirstChild();
    return 0;
}
