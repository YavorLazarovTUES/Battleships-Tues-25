#ifndef SDES_H
#define SDES_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern int P10[];
extern int P8[];
extern int IP[];
extern int IP_INV[];
extern int EP[];
extern int P4[];
extern int S0[4][4];
extern int S1[4][4];

uint16_t permute10(uint16_t input, int *perm);
uint8_t permute8(uint8_t input, int *perm, int n);
uint16_t left_rotate_5(uint16_t input, int n);
void generate_keys(uint16_t key, uint8_t *K1, uint8_t *K2);
uint8_t sbox_lookup(uint8_t input, int sbox[4][4]);
uint8_t fk(uint8_t half, uint8_t subkey);
uint8_t sdes(uint8_t input, uint8_t K1, uint8_t K2, int mode);
void sdes_dec(char filename[100], uint16_t key);
void sdes_enc(char filename[100], uint16_t key);

#endif