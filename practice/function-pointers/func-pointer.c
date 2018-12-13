#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef
double (*unary_real_function_t)(double);

unary_real_function_t funcs[] = { &sqrt, exp, log, NULL };

int main() {
    double x = 100;
    unary_real_function_t func = NULL;
    double y;
    for (int i=0; funcs[i]; ++i) {
        func = funcs[i];
        y = func(x);
        printf("func(%g) = %g\n", x, y);
    }
}
