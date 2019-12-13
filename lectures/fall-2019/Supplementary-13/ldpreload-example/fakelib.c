#define _GNU_SOURCE
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>

typedef void (*sighandler_t)(int);

typedef int (*sigaction_ptr_t)
(
 int,
 const struct sigaction *restrict,
 struct sigaction *
);

int sigaction(int signum,
                      const struct sigaction *restrict act,
                      struct sigaction *oldact)
{
    puts("Called fake sigaction\n");
    
    // Make additional work time to simulate
    // race condition
    sleep(3);
    static sigaction_ptr_t real_sigaction = NULL;
    if (NULL == real_sigaction) {        
        real_sigaction = dlsym(RTLD_NEXT, "sigaction");
        if (!real_sigaction) {
            fputs(dlerror(), stderr);
            _exit(1);
        }
    }
    return real_sigaction(signum, act, oldact);
}
