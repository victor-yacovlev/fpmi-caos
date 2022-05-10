#include <stdio.h>
#include <unistd.h>

void some_func() {
    puts("I'm some function");
}

int main() {
    printf("Started some program with PID = %d\n", getpid());
    while (1) {
        getchar(); // like pause
        some_func();
    }
}
