#include <stdlib.h>
#include <stdio.h>

int main()
{
    int * ptr = malloc(16ULL*1024*1024*1024); // 16 Gb
    puts("malloc called");
    getchar();
    ptr[0] = 123;
}
