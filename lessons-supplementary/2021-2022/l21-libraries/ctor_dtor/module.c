#include <stdio.h>

void function() {
    puts("I'm a function into module");
}

__attribute__((constructor))
static void initialize() {
    puts("I'm a module constructor, called once");
}

__attribute__((destructor))
static void finalize() {
    puts("I'm a module destructor, it's cool that I'm called");
}
