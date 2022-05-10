#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    const char *shm_name = argv[1];
    int fd = shm_open(shm_name, O_RDWR|O_CREAT, 0600);
    if (-1 == fd) {
        perror("SHM creation failed");
        return 1;
    }
    close(fd);
    printf("SHM created, press ENTER to release\n");
    getchar();
    shm_unlink(shm_name);
}
