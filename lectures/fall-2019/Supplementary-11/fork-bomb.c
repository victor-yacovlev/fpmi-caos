#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

int main()
{
    char * are_you_sure = getenv("ALLOW_FORK_BOMB");
    if (!are_you_sure || 0!=strcmp(are_you_sure, "yes")) {
        fprintf(stderr, "Fork bomb not allowed!\n");
        exit(127);
    }

    pid_t pid;
    do {
        pid = fork();
    } while (-1 != pid);

    printf("Process %d reached out limit on processes\n", getpid());
    while (1) {
        sched_yield();
        sleep(1);
    }
}
