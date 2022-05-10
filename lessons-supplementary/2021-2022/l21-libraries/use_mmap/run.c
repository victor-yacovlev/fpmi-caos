#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef double (*func_t)(double);

int main(int argc, char *argv[]) {
    const char *file_name = argv[1];
    double argument = strtod(argv[2], NULL);

    int fd = open(file_name, O_RDONLY);
    struct stat st = {};
    fstat(fd, &st);
    func_t func = mmap(NULL, st.st_size,
                       PROT_READ|PROT_EXEC,
                       MAP_PRIVATE, fd, 0);
    close(fd);
    if (MAP_FAILED == func) { perror("mmap failed"); exit(1); }
    double result = func(argument);
    printf("func(%f) = %f\n", argument, result);
    munmap(func, st.st_size);
}
