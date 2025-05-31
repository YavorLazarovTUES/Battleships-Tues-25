#ifndef BTLSHP_H
#define BTLSHP_H

#include <stdint.h>

typedef struct{
    uint8_t board[10][10];//0->empty; 1-10->ship_alive_cell_id; 11-20->ship_hit_id;21-30->ship_destroyed_id; 32 - miss
}Board_State;

typedef struct{
    int x,y;
}Coords;

typedef struct{
    Coords c;
    int dir;//0=N;1=S;2=W;3=E;
}Dir_coords;

typedef struct{
    Dir_coords Ship_dir;
    int ship_len;
}Ship_pos;


int fire(Board_State* tgt_b_st,Coords trgt); // vrushta 0=miss; 1=hit; 2=destroyed; 3=win
// if hit -> primer: Purvi korab -> ot id 1 stava na id 11 ako ima oshte id 1 ostava id 11 ako ne vsichki id 11 stavat id 21

int board_verificator(Board_State* cur_b_st, Dir_coords new_boat, int len, int ship_id);//0=fail; 1=success;
// cur_b_st - current board state 

#endif