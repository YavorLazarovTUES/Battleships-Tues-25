#ifndef REPLAY_H
#define REPLAY_H

#include <stdint.h>
void Write(char filename[100], uint8_t player_1_replay[10][10], uint8_t player_2_replay[10][10]);
void Writecoords(char filename[100], int target, int x, int y);
void playReplay(char filename[100]);
#endif