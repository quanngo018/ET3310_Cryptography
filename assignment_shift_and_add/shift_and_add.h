#ifndef SHIFT_AND_ADD_H
#define SHIFT_AND_ADD_H

// Nhan a(x) * b(x) mod m(x) trong GF(2^n) bang thuat toan Shift-and-Add.
inline int shift_and_add(int a, int b, int m) {
    int result = 0;
    while (b > 0) {
        if (b & 1) {
            result = result ^ a;
        }
        a <<= 1;
        if(a & 0x100) {
            a ^= m;
        }
        b >>= 1;
        if (b == 0) {
            break;
        }
    }
    return result;
}

#endif
