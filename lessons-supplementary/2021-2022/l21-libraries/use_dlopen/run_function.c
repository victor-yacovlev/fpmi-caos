#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>

typedef double (*func_t)(double);

int main(int argc, char* argv[]) {
    const char *lib_name = argv[1];
    const char *func_name = argv[2];
    double argument = strtod(argv[3], NULL);

    puts("Press Enter to load library.");
    getchar(); // like pause
    
    void *lib = dlopen(lib_name, RTLD_NOW|RTLD_LOCAL);
    if (NULL == lib) {
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
        exit(1);
    }
    else {
        printf("Library %s loaded at %zx. Press Enter to continue.\n",
               lib_name, lib);
        getchar(); // like pause
    }
    
    func_t func = dlsym(lib, func_name);
    if (NULL == func) {
        fprintf(stderr, "Failed to find function: %s\n", dlerror());
        exit(1);
    }
    else {
        printf("Found %s in library at %zx. Press Enter to continue.\n",
               func_name, func);
        getchar(); // like pause
    }
    
    double value = func(argument);
    printf("func(%f) = %f\n", argument, value);

    dlclose(lib);
}
