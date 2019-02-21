#include <signal.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t int_received = 0;
volatile sig_atomic_t term_received = 0;

void sigint_handler(int num) { int_received ++; }
void sigterm_handler(int num) { term_received = 1;}

int main() {
    // Register handlers
    sigaction(SIGINT, &(struct sigaction) {
                .sa_handler = sigint_handler,
                .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) {
                .sa_handler = sigterm_handler,
                .sa_flags = SA_RESTART }, NULL);
    
    // Block SIGINT
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    while ( ! term_received ) {
        pause();
    }
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
    printf("Got %d times SIGINT\n", int_received);
}
