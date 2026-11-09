#include <cstdio>
#include "shift_and_add.h"

int main() {
    int a = 0x05; // x^2 + 1
    int b = 0x03; // x + 1
    int m = 0x13; // x^4 + x + 1

    int result = shift_and_add(a, b, m);
    printf("Result: 0x%X\n", result);
}
