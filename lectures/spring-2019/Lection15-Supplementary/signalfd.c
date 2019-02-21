#include <sys/signalfd.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    // Prepare set of signals
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);

    // Disable signals handling (by block)
    sigprocmask(SIG_BLOCK, &mask, NULL);
        
    int fd = signalfd(-1, &mask, 0);
    
    struct signalfd_siginfo info;
    while (read(fd, &info, sizeof(info)) > 0) {
        printf("Got signal %d from PID %d\n",
               info.ssi_signo, info.ssi_pid);
    }
    
}
