#include <unistd.h>

int main() {
    static const char Hello1[] =
        "Hello, STDOUT!\n";
    static const char Hello2[] =
        "Hello, STDERR!\n";
    static const char Hello795[] =
        "Hello, group 795!\n";
    write(  1, Hello1  , sizeof(Hello1  )-1);
    write(  2, Hello2  , sizeof(Hello2  )-1);
    write(795, Hello795, sizeof(Hello795)-1);
}
