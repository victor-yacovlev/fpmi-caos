#include <stdio.h>

void
callable_function()
{
    puts("The function that might be called");
}

int
main(int argc, char *argv[])
{
    printf("Argc = %d\n", argc);
    return 100;
}
