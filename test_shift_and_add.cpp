// Unit test cho shift_and_add (nhan trong GF(2^n)).
// Bien dich: g++ -std=c++17 -Wall -o test_shift_and_add test_shift_and_add.cpp
#include <cstdio>
#include "shift_and_add.h"

static int g_pass = 0;
static int g_fail = 0;

// Bac cua da thuc p(x) = vi tri bit cao nhat.
static int degree(int p) {
    int d = -1;
    while (p) { p >>= 1; d++; }
    return d;
}

// Ham tham chieu (oracle doc lap): nhan da thuc khong nho truoc, roi moi chia lay du.
static int ref_mul(int a, int b, int m) {
    long long prod = 0;
    for (int i = 0; b >> i; i++)
        if ((b >> i) & 1) prod ^= (long long)a << i;
    int dm = degree(m);
    for (int i = 2 * dm; i >= dm; i--)
        if ((prod >> i) & 1) prod ^= (long long)m << (i - dm);
    return (int)prod;
}

static void check_eq(int a, int b, int m, int expected, const char* note) {
    int got = shift_and_add(a, b, m);
    if (got == expected) {
        g_pass++;
        printf("  PASS  0x%02X * 0x%02X mod 0x%X = 0x%02X   (%s)\n", a, b, m, got, note);
    } else {
        g_fail++;
        printf("  FAIL  0x%02X * 0x%02X mod 0x%X = 0x%02X, expected 0x%02X   (%s)\n",
               a, b, m, got, expected, note);
    }
}

// Kiem toan bo cac cap (a, b) < 2^n: so voi oracle, giao hoan, ket qua < 2^n.
static void exhaustive(int m) {
    int n = degree(m), size = 1 << n;
    int bad_ref = 0, bad_comm = 0, bad_range = 0;
    int shown = 0;
    for (int a = 0; a < size; a++) {
        for (int b = 0; b < size; b++) {
            int got = shift_and_add(a, b, m);
            int exp = ref_mul(a, b, m);
            if (got != exp) {
                bad_ref++;
                if (shown++ < 5)
                    printf("    vd sai: 0x%02X * 0x%02X = 0x%X, expected 0x%02X\n", a, b, got, exp);
            }
            if (got != shift_and_add(b, a, m)) bad_comm++;
            if (got < 0 || got >= size) bad_range++;
        }
    }
    int total = size * size;
    auto report = [&](const char* name, int bad) {
        if (bad == 0) { g_pass++; printf("  PASS  %-28s %d/%d cap\n", name, total, total); }
        else          { g_fail++; printf("  FAIL  %-28s %d/%d cap sai\n", name, bad, total); }
    };
    report("khop voi ref_mul", bad_ref);
    report("giao hoan a*b == b*a", bad_comm);
    report("ket qua < 2^n", bad_range);
}

int main() {
    const int GF16 = 0x13;   // x^4 + x + 1
    const int AES  = 0x11B;  // x^8 + x^4 + x^3 + x + 1

    printf("[GF(2^4), m = 0x13] Cac ca tinh tay\n");
    check_eq(0x07, 0x00, GF16, 0x00, "nhan 0");
    check_eq(0x00, 0x07, GF16, 0x00, "0 nhan b");
    check_eq(0x07, 0x01, GF16, 0x07, "don vi");
    check_eq(0x01, 0x07, GF16, 0x07, "1 nhan b");
    check_eq(0x05, 0x03, GF16, 0x0F, "khong can rut gon");
    check_eq(0x08, 0x02, GF16, 0x03, "x^4 = x+1, can rut gon");
    check_eq(0x0F, 0x0F, GF16, 0x0A, "can rut gon");
    check_eq(0x09, 0x0B, GF16, 0x0C, "can rut gon");
    check_eq(0x02, 0x09, GF16, 0x01, "nghich dao");

    printf("\n[GF(2^8), m = 0x11B] Vector chuan FIPS-197\n");
    check_eq(0x57, 0x83, AES, 0xC1, "FIPS-197 4.2");
    check_eq(0x57, 0x13, AES, 0xFE, "FIPS-197 4.2.1");
    check_eq(0x53, 0xCA, AES, 0x01, "nghich dao");

    printf("\n[GF(2^4), m = 0x13] Exhaustive\n");
    exhaustive(GF16);
    printf("\n[GF(2^8), m = 0x11B] Exhaustive\n");
    exhaustive(AES);

    printf("\nTong ket: %d PASS, %d FAIL\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
