#ifndef AI_H
#define AI_H
extern char filename[];
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "replay.h"
#include "Btlshp.h"
Board_State* generate_ai_board();

typedef struct {
    int state;  // 0-random 1-circling, 2- following
    Coords first_hit;
    Coords last_hit;
    char direction;  // N, S, W, E
    int reverse;     // 0-normal, 1- reverse
    int moves[10][10]; //0-not tried, 1-tried
} AI_State;

AI_State init_ai_state();

Coords get_ai_target(Board_State* player_board, AI_State* ai_state);

void update_ai_state(AI_State* ai_state, Coords target, int hit_result);
void game_loop_1p(Board_State* player_board, Board_State* ai_board);
#endif