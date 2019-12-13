#include <stdio.h>

__attribute__((constructor))
void initialize_fakelib() {
    puts("Fake library initialized");
}

__attribute__((destructor))
void finalize_fakelib() {
    puts("Fake library unloading");
}
