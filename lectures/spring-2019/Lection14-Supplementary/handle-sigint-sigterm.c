#include <signal.h>
#include <sched.h>
#include <stdio.h>

void handler(int signum) {
    // Warning: govnokod!
    printf("Caught signal %d\n", signum);
}

int main() {
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    while (1) sched_yield();
}
