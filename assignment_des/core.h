#include <stdint.h>
#ifndef CORE_H
#define CORE_H

void initial_permutation(const uint64_t input, uint64_t *output);
void split_left_right(const uint64_t input, uint32_t *left, uint32_t *right);
void feistel_function(const uint32_t right, const uint64_t subkey, uint32_t *output);
void feistel_round(uint32_t *left, uint32_t *right, const uint64_t subkey);
void des_encrypt(const uint64_t plaintext, const uint64_t *subkeys, uint64_t *ciphertext);
void swap_32(uint32_t *a, uint32_t *b);
void inverse_initial_permutation(const uint64_t input, uint64_t *output);

#endif