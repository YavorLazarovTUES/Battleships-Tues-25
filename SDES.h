#ifndef SDES_H
#define SDES_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

uint16_t permute10(uint16_t input, int *perm);
uint8_t permute8(uint8_t input, int *perm, int n);
uint16_t left_rotate_5(uint16_t input, int n);
void generate_keys(uint16_t key, uint8_t *K1, uint8_t *K2);
uint8_t sbox_lookup(uint8_t input, int sbox[4][4]);
uint8_t fk(uint8_t half, uint8_t subkey);
uint8_t sdes(uint8_t input, uint8_t K1, uint8_t K2, int mode);

#endif