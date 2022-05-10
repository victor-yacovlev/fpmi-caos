#include <stdio.h>

#include <dlfcn.h>

void cat() { puts("Meaow"); }
void dog() { puts("Huff!"); }

typedef void (*func_t)();

int main() {
    char name[4096] = {};
    printf("Please enter animal kind: ");
    scanf("%s", name);

    void *myself = dlopen(NULL, RTLD_NOW|RTLD_GLOBAL);
    func_t func = dlsym(myself, name);

    func();
}
