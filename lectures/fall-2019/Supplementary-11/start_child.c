#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("abrakadabra "); // write
  fflush(stdout); //
  pid_t result = fork();
  if (0==result) {
      printf("I'm son\n"); // MINIX, QNX 
  }
  else {
      printf("I'm parent\n");
  }
}
