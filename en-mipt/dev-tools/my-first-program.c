/* my-first-program.c */
#include <stdio.h>

static void
do_something()
{
    printf("Hello, World!\n");
}

extern void
do_something_else(int value)
{
    printf("Number is %d\n", value);
}

int
main()
{
    do_something();
}
