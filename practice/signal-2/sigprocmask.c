#include <signal.h>
#include <unistd.h>

static void
handler(int signum) {
    static const char Message[] = "Got Ctrl+C\n";
    write(1, Message, sizeof(Message)-1);
}

int main() {
    sigaction(SIGINT,
              &(struct sigaction)
              {.sa_handler=handler, .sa_flags=SA_RESTART},
              NULL);
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    while (1) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        sleep(10);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
    }
}
