#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t caught_signum = 0;

void handler(int signum) {
    // Do not invoke printf, but just
    // save signal number
    caught_signum = signum;
}

int main() {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;

    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGINT, &act, NULL);

    while (1) {
        pause(); // wait until signal caught and processed
        printf("Got signal %d\n", caught_signum);
    }
        
}
