#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    uint64_t stack_size = strtoull(argv[1], NULL, 10);
    stack_size *= 1024; // convert from KB to bytes
    pid_t pid = fork();
    if (-1==pid) { perror("fork"); exit(1); }
    if (0==pid) {
        // set new limits before exec
        struct rlimit rlim;
        getrlimit(RLIMIT_STACK, &rlim);
        if (RLIM_INFINITY==rlim.rlim_max || rlim.rlim_max>stack_size) {
            rlim.rlim_cur = stack_size;
        }
        else {
            rlim.rlim_cur = rlim.rlim_max;
        }
        setrlimit(RLIMIT_STACK, &rlim);
        execlp("bash", "bash", NULL);
        /* perror("exec"); --- can't use stdlib after stack size change */ 
        _exit(1);
    }
    else {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        return WEXITSTATUS(wstatus);
    }
}
