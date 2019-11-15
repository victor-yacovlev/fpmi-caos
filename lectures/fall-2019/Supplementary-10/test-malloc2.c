#include <stdlib.h>

int main()
{
    void *ptr = malloc(99999999);
    free(ptr);
    free(ptr);
}
