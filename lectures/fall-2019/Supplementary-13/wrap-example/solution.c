#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handle_sigint(int signum)
{
    static const char Message[] = "Caught SIGINT\n";
    write(1, Message, sizeof(Message)-1);
}

int main()
{
    // Wrong solution: race condition
    printf("My PID is %d\n", getpid());
    fflush(stdout);
    
    struct sigaction action_int;
    memset(&action_int, 0, sizeof(action_int));
    action_int.sa_handler = handle_sigint;
    action_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action_int, NULL);
    
    while (1) pause();
}
