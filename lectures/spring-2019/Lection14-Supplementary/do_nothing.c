#include <sched.h>

int main()
{
    while (1) {
        sched_yield();
    }
}
