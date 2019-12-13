#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

int __real_sigaction(int signum,
                      const struct sigaction *restrict act,
                      struct sigaction *oldact);
int __wrap_sigaction(int signum,
                      const struct sigaction *restrict act,
                      struct sigaction *oldact)
{
    // Make additional work time to simulate
    // race condition
    sleep(3);
    return __real_sigaction(signum, act, oldact);
}
