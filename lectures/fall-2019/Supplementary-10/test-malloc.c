#include <stdlib.h>

int main()
{
    void *ptr = malloc(1);
    free(ptr);
    free(ptr);
}
