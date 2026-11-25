#include "key_generate.h"
#include "des_table.h"

void generate_subkeys(const uint64_t key, uint64_t *subkeys) {
    uint64_t permuted_key = 0;
    for (int i = 0; i < 56; i++) {
        permuted_key |= ((key >> (64 - PC1[i])) & 1ULL) << (55 - i);
    }

    uint32_t C = (uint32_t)((permuted_key >> 28) & 0x0FFFFFFF);
    uint32_t D = (uint32_t)(permuted_key & 0x0FFFFFFF);

    for (int round = 0; round < 16; round++) {
        int shift = SHIFTS[round];
        C = ((C << shift) | (C >> (28 - shift))) & 0x0FFFFFFF;
        D = ((D << shift) | (D >> (28 - shift))) & 0x0FFFFFFF;

        uint64_t CD = ((uint64_t)C << 28) | D;

        uint64_t subkey = 0;
        for (int i = 0; i < 48; i++) {
            subkey |= ((CD >> (56 - PC2[i])) & 1ULL) << (47 - i);
        }
        subkeys[round] = subkey;
    }
}
