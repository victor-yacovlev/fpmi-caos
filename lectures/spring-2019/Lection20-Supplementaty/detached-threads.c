#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void*
thread_function(void* arg)
{
    while (1) {
        printf("Thread %llu is running\n", (size_t)arg);
        sleep(1);
    }
}

int main()
{
    const size_t N = 10;
    pthread_t threads[N];
    for (size_t i=0; i<N; ++i) {
        pthread_create(&threads[i],
                       NULL,
                       thread_function,
                       (void*) i);
    }
    pthread_exit(0);
}
