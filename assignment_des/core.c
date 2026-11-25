#include "core.h"
#include "des_table.h"

void swap_32(uint32_t *a, uint32_t *b) {
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

void split_left_right(const uint64_t input, uint32_t *left, uint32_t *right) {
    *left = (uint32_t)(input >> 32);
    *right = (uint32_t)(input & 0xFFFFFFFF);
}

void initial_permutation(const uint64_t input, uint64_t *output) {
    *output = 0;
    for (int i = 0; i < 64; i++) {
        *output |= ((input >> (64 - IP[i])) & 1) << (63 - i);
    }
}

void inverse_initial_permutation(const uint64_t input, uint64_t *output) {
    *output = 0;
    for (int i = 0; i < 64; i++) {
        *output |= ((input >> (64 - INV_IP[i])) & 1) << (63 - i);
    }
}

void feistel_function(const uint32_t right, const uint64_t subkey, uint32_t *output) {
    uint64_t expanded_right = 0;
    for (int i = 0; i < 48; i++) {
        expanded_right |= ((uint64_t)((right >> (32 - E[i])) & 1)) << (47 - i);
    }

    expanded_right ^= subkey;

    *output = 0;
    for (int i = 0; i < 8; i++) {
        int row = (((expanded_right >> (47 - 6 * i)) & 0x01) << 1)
        | ((expanded_right >> (42 - 6 * i)) & 0x01);
        int col = (expanded_right >> (42 - 6 * i + 1)) & 0x0F;
        *output |= ((uint32_t)S_BOX[i][row][col]) << (28 - 4 * i);
    }

    uint32_t permuted_output = 0;
    for (int i = 0; i < 32; i++) {
        permuted_output |= ((*output >> (32 - P[i])) & 1) << (31 - i);
    }
    *output = permuted_output;
}

void feistel_round(uint32_t *left, uint32_t *right, const uint64_t subkey) {
    uint32_t temp = *right;
    feistel_function(*right, subkey, right);
    *right ^= *left;
    *left = temp;
}

void des_encrypt(const uint64_t plaintext, const uint64_t *subkeys, uint64_t *ciphertext) {
    uint64_t permuted_input;
    initial_permutation(plaintext, &permuted_input);

    uint32_t left, right;
    split_left_right(permuted_input, &left, &right);

    for (int i = 0; i < 16; i++) {
        feistel_round(&left, &right, subkeys[i]);
    }

    swap_32(&left, &right);

    uint64_t pre_output = ((uint64_t)left << 32) | right;
    inverse_initial_permutation(pre_output, ciphertext);
}


