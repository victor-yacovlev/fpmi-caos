#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*func_t)(int);

int main()
{
    void * lib = dlopen("libmylib.so", RTLD_NOW);
    if (! lib) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        exit(1);
    }
    void * entry = dlsym(lib, "some_func");
    if (! entry) {
        fprintf(stderr, "dlsym error: %s\n", dlerror());
        exit(1);
    }
    func_t func = entry;
    func(123);
    dlclose(lib);
}
