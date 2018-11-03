#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int
main()
{
    close(2);
    close(1);
    /* 1 = */ open("out.txt", O_WRONLY | O_CREAT, 0640);
    /* 2 = */ open("err.txt", O_WRONLY | O_CREAT, 0640);
    printf("Hello!");
    fprintf(stderr, "World");
    static const char buffer3[] = "Number three";
    static const char buffer4[] = "Number four";
    write(3, buffer3, sizeof(buffer3)-1);
    lseek(4, 100, SEEK_END);
    write(4, buffer4, sizeof(buffer4)-1);
}
