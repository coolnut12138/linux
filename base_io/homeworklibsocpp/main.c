#include <stdio.h>
#include "arithmetic.h"

int main()
{
    int a = 1, b = 2;
    printf("a + b = %d\n", add(a, b));
    printf("a - b = %d\n", sub(a, b));
    printf("a * b = %d\n", mul(a, b));
    printf("a / b = %d\n", div(a, b));
    return 0;
}
