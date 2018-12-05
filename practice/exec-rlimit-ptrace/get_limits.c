#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>

static void
print_limit(int val, const char *name)
{
    struct rlimit rlim;
    memset(&rlim, 0, sizeof(rlim));
    getrlimit(val, &rlim);
    printf("%-15s│ ", name);
    if (RLIM_INFINITY==rlim.rlim_cur)
        printf("∞         │ ");
    else if (4==sizeof(void*))
        printf("%-10u│ ", rlim.rlim_cur);
    else
        printf("%-10llu│ ", rlim.rlim_cur);
    if (RLIM_INFINITY==rlim.rlim_max)
        printf("∞\n");
    else if (4==sizeof(void*))
        printf("%-11u\n", rlim.rlim_max);
    else
        printf("%-11llu\n", rlim.rlim_max);
    
}

int main() {
    printf("───────────────┬───────────┬────────────\n");
    printf("     Name      │   Soft    │    Hard    \n");
    printf("───────────────┼───────────┼────────────\n");
    print_limit(RLIMIT_AS, "RLIMIT_AS");
    print_limit(RLIMIT_CPU, "RLIMIT_CPU");
    print_limit(RLIMIT_DATA, "RLIMIT_DATA");
    print_limit(RLIMIT_FSIZE, "RLIMIT_LOCKS");
    print_limit(RLIMIT_NICE, "RLIMIT_NICE");
    print_limit(RLIMIT_NOFILE, "RLIMIT_NOFILE");
    print_limit(RLIMIT_RSS, "RLIMIT_RSS");
    print_limit(RLIMIT_STACK, "RLIMIT_STACK");
    printf("───────────────┴───────────┴────────────\n");
}
