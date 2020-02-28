#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

#include <pthread.h>
#include <unistd.h>


static void*
thread_function(void* arg)
{
    while (1) {
        printf("Thread %"PRIu64" is running\n", (uint64_t) arg);
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
    //pthread_exit(0);
}
