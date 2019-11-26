#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>

void handle_with_one_arg(int signum)
{
    // a bit better, but still govnokod
    printf("Got signal %d\n", signum);
}

void
handle_with_three_args(int signum, siginfo_t *info, void *ctx)
{
    // govnokod is still here
    printf("Got signal %d from process %d\n", signum, info->si_pid);
}

int main() {
    struct sigaction int_handler;
    memset(&int_handler, 0, sizeof(int_handler));

    int_handler.sa_handler = handle_with_one_arg;
    int_handler.sa_flags = SA_RESTART;
    sigaction(SIGINT, &int_handler, NULL);

    struct sigaction term_handler;
    memset(&term_handler, 0, sizeof(term_handler));

    term_handler.sa_sigaction = handle_with_three_args;
    term_handler.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGTERM, &term_handler, NULL);

    while (1) sched_yield();
}
