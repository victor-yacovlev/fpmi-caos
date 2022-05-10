#include <stdio.h>
#include <unistd.h>

__attribute__((section(".interp")))
const char service_interp[]
              = "/lib/ld-linux-aarch64.so.1";

void function() {
    puts("I'm a function!");
}

void custom_start() {
    function();
    _exit(0);
}
