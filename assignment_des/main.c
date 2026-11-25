#include <stdio.h>
#include <stdint.h>
#include "core.h"
#include "key_generate.h"

int main(void) {
    uint64_t key = 0x133457799BBCDFF1ULL;
    uint64_t plaintext = 0x0123456789ABCDEFULL;
    uint64_t expected_ciphertext = 0x85E813540F0AB405ULL;

    uint64_t subkeys[16];
    generate_subkeys(key, subkeys);

    uint64_t ciphertext;
    des_encrypt(plaintext, subkeys, &ciphertext);

    printf("Key        : %016llX\n", (unsigned long long)key);
    printf("Plaintext  : %016llX\n", (unsigned long long)plaintext);
    printf("Ciphertext : %016llX\n", (unsigned long long)ciphertext);
    printf("Expected   : %016llX\n", (unsigned long long)expected_ciphertext);
    printf("%s\n", (ciphertext == expected_ciphertext) ? "PASS" : "FAIL");

    return 0;
}
