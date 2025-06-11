#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "SDES.h"

int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[]  = {6, 3, 7, 4, 8, 5, 10, 9};
int IP[]  = {2, 6, 3, 1, 4, 8, 5, 7};
int IP_INV[] = {4, 1, 3, 5, 7, 2, 8, 6};
int EP[]  = {4, 1, 2, 3, 2, 3, 4, 1};
int P4[]  = {2, 4, 3, 1};

int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

uint16_t permute10(uint16_t input, int *perm) {
    uint16_t output = 0;
    for (int i = 0; i < 10; i++) {
        output <<= 1;
        output |= (input >> (10 - perm[i])) & 1;
    }
    return output;
}

uint8_t permute8(uint8_t input, int *perm, int n) {
    uint8_t output = 0;
    for (int i = 0; i < n; i++) {
        output <<= 1;
        output |= (input >> (8 - perm[i])) & 1;
    }
    return output;
}

uint16_t left_rotate_5(uint16_t input, int n) {
    return ((input << n) | (input >> (5 - n))) & 0x1F; // само 5 бита
}

void generate_keys(uint16_t key, uint8_t *K1, uint8_t *K2) {
    key = permute10(key, P10);

    uint16_t left = (key >> 5) & 0x1F;
    uint16_t right = key & 0x1F;

    left = left_rotate_5(left, 1);
    right = left_rotate_5(right, 1);

    uint16_t combined = (left << 5) | right;
    *K1 = permute8((uint8_t)(combined >> 2), P8, 8);

    left = left_rotate_5(left, 2);
    right = left_rotate_5(right, 2);
    combined = (left << 5) | right;
    *K2 = permute8((uint8_t)(combined >> 2), P8, 8);
}

uint8_t sbox_lookup(uint8_t input, int sbox[4][4]) {
    int row = ((input & 0x8) >> 2) | (input & 0x1);  // първи и последен бит
    int col = (input & 0x6) >> 1;                     // средните два бита
    return sbox[row][col];
}

uint8_t fk(uint8_t half, uint8_t subkey) {
    uint8_t expanded = permute8(half, EP, 8);
    uint8_t xored = expanded ^ subkey;

    uint8_t left = (xored >> 4) & 0x0F;
    uint8_t right = xored & 0x0F;

    uint8_t s0_val = sbox_lookup(left, S0);
    uint8_t s1_val = sbox_lookup(right, S1);

    uint8_t combined = (s0_val << 2) | s1_val;
    return permute8(combined, P4, 4);
}

uint8_t sdes(uint8_t input, uint8_t K1, uint8_t K2, int mode) {
    uint8_t ip = permute8(input, IP, 8);

    uint8_t left = (ip >> 4) & 0x0F;
    uint8_t right = ip & 0x0F;

    if (mode == 0) { //криптиране
        left ^= fk(right, K1);
        uint8_t temp = left;
        left = right;
        right = temp;

        left ^= fk(right, K2);
    } else { // декриптиране
        left ^= fk(right, K2);
        uint8_t temp = left;
        left = right;
        right = temp;

        left ^= fk(right, K1);
    }

    uint8_t preoutput = (left << 4) | right;
    return permute8(preoutput, IP_INV, 8);
}

void sdes_enc(char filename[100], uint16_t key) {
    FILE *in = fopen(filename, "r");
    if (!in) {
        printf("Error opening file to encrypt\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file\n");
        fclose(in);
        return;
    }

    uint8_t K1, K2;
    generate_keys(key, &K1, &K2);

    char line[1024];
    while (fgets(line, sizeof(line), in)) {
        char *ptr = line;
        int value;

        while (sscanf(ptr, "%d", &value) == 1) {
            uint8_t encrypted = sdes((uint8_t)value, K1, K2, 0);
            fprintf(temp, "%d ", encrypted);

            while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
        }
        fprintf(temp, "\n");
    }

    fclose(in);
    fclose(temp);

    remove(filename);
    rename("temp.txt", filename);
}

void sdes_dec(char filename[100], uint16_t key) {
    FILE *in = fopen(filename, "r");
    if (!in) {
        printf("Error opening file to decrypt\n");
        return;
    }

    FILE *out = fopen("decrypted.txt", "w");
    if (!out) {
        printf("Error opening output file\n");
        fclose(in);
        return;
    }

    uint8_t K1, K2;
    generate_keys(key, &K1, &K2);

    char line[1024];
    while (fgets(line, sizeof(line), in)) {
        char *ptr = line;
        int value;

        while (sscanf(ptr, "%d", &value) == 1) {
            uint8_t decrypted = sdes((uint8_t)value, K1, K2, 1);
            fprintf(out, "%d ", decrypted);

            while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
        }
        fprintf(out, "\n");
    }

    fclose(in);
    fclose(out);
}


