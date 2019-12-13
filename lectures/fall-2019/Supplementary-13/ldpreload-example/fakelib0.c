#define _GNU_SOURCE
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
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
}
