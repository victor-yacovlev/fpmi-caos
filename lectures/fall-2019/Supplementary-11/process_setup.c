#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t  pid = fork();
    if (-1==pid) { perror("fork :-("); exit(1); }
    if (0==pid) {
      chdir("/usr/bin");
      int fd = open("/tmp/out.txt",
         	  O_WRONLY|O_CREAT|O_TRUNC, 0644);
    dup2(fd, 1); close(fd);
    execlp("ls", "ls", "-l", NULL);
    perror("exec :-(");
    exit(2);
  }
  else {
    waitpid(pid, NULL, 0);
  }
}

