#include <signal.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t int_received = 0;

void sigint_handler(int num) { int_received ++; }

int main() {
    // Register handler
    sigaction(SIGINT, &(struct sigaction) {
                .sa_handler = sigint_handler,
                .sa_flags = SA_RESTART }, NULL);
    
    // Block SIGINT
    sigset_t set_with_int;
    sigemptyset(&set_with_int);
    sigaddset(&set_with_int, SIGINT);
    sigprocmask(SIG_BLOCK, &set_with_int, NULL);
    
    // Temporary unblock SIGINT and wait it
    sigset_t set_withOUT_int;
    sigfillset(&set_withOUT_int);
    sigdelset(&set_withOUT_int, SIGINT);
    sigsuspend(&set_withOUT_int);
    
    printf("Got %d times SIGINT\n", int_received);
}
