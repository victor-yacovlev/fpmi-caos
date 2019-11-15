#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("abrakadabra ");
  pid_t result = fork();
  if (0==result) {
    printf("I'm son\n");
  }
  else {
    printf("I'm parent\n");
  }
}
