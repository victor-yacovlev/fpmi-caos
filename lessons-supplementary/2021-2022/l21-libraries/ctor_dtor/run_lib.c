#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>

typedef void (*func_t)();

int main(int argc, char *argv[]) {
    const char *lib_name = argv[1];
    const char *func_name = argv[2];

    printf("Loaing library %s\n", lib_name);
    void *lib = dlopen(lib_name, RTLD_LAZY|RTLD_LOCAL);
    if (NULL == lib) {
        fprintf(stderr, "Cant load libary %s: %s\n",
                lib_name, dlerror());
        exit(1);
    }
    printf("Loaded library %s\n", lib_name);

    func_t func = dlsym(lib, func_name);
    if (NULL == func) {
        fprintf(stderr, "Cant find symbol %s: %s\n",
                func_name, dlerror());
        exit(1);
    }

    func();

    printf("Unloading library %s\n", lib_name);
    if (0 == dlclose(lib)) {
        printf("Unloaded library %s\n", lib_name);
    } else {
        printf("Library %s still in use and not unloaded\n", lib_name);
    }
}
