#include "Btlshp.h"
int check_for_destroyed(Board_State* tgt_b_st,uint8_t trgt){
    Coords ship_det[6];
    int k=0;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(tgt_b_st->board[i][j]==trgt)return 0;
            if(tgt_b_st->board[i][j]==trgt+10){
                ship_det[k].x=i;
                ship_det[k].y=j;
                k++;
            }
        }
    }
    for(int i=0;i<=k;i++)tgt_b_st->board[ship_det[i].x][ship_det[i].y]=trgt+20;
    return 1;
}
int check_for_win(Board_State* tgt_b_st){
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(1<=tgt_b_st->board[i][j] && tgt_b_st->board[i][j]<=10)return 0;
        }
    }
    return 1;
}

int fire(Board_State* tgt_b_st,Coords trgt){
    uint8_t* target=tgt_b_st->board[trgt.x][trgt.y];
    if(*target==0){*target=32;return 0;}
    else{
        if(1<=*target && *target<=10){
            *target+=10;
            if(check_for_destroyed(tgt_b_st,target-10)){
                if(check_for_win(tgt_b_st)) return 3;
                else return 2;
            }
        }
        return 1;
    }
}

int board_verificator(Board_State* cur_b_st, Dir_coords new_boat, int len){
    
}