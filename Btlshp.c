#include "Btlshp.h"
#include <stdio.h>
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
    if(tgt_b_st->board[trgt.x][trgt.y]==0){tgt_b_st->board[trgt.x][trgt.y]=32;return 0;}
    else{
        if(1<=tgt_b_st->board[trgt.x][trgt.y] && tgt_b_st->board[trgt.x][trgt.y]<=10){
            tgt_b_st->board[trgt.x][trgt.y]+=10;
            if(check_for_destroyed(tgt_b_st,tgt_b_st->board[trgt.x][trgt.y]-10)){
                if(check_for_win(tgt_b_st)) return 3;
                else return 2;
            }
        }
        return 1;
    }
}

int b_v_check_coords(Board_State* cur_b_st,Coords loc){
    if(loc.x<0||loc.x>10||loc.y<0||loc.y>10) return 0;
    return cur_b_st->board[loc.x][loc.y];
}
int b_v_N(Board_State* cur_b_st, Coords loc, int len,int id){
    Coords tmp_loc;
    for(int x=loc.x-1;x<=loc.x+1;x++){
        tmp_loc.x=x;
        for(int y=loc.y-len;y<=loc.y-1;y++){
            tmp_loc.y=y;
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0){
                printf("Can't place ship there!");
                return 0;
            }
        }
    }
    for(int y=loc.y;y<loc.y-len;y--)cur_b_st->board[loc.x][y]=id;
    return 1;
}
int b_v_S(Board_State* cur_b_st, Coords loc, int len,int id){
    Coords tmp_loc;
    for(int x=loc.x-1;x<=loc.x+1;x++){
        tmp_loc.x=x;
        for(int y=loc.y-1;y<=loc.y+len;y++){
            tmp_loc.y=y;
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0){
                printf("Can't place ship there!");
                return 0;
            }
        }
    }
    for(int y=loc.y;y<loc.y+len;y++)cur_b_st->board[loc.x][y]=id;
    return 1;
}
int b_v_W(Board_State* cur_b_st, Coords loc, int len,int id){
    Coords tmp_loc;
    for(int x=loc.x-len;x<=loc.x+1;x++){
        tmp_loc.x=x;
        for(int y=loc.y+1;y>=loc.y-1;y--){
            tmp_loc.y=y;
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0){
                printf("Can't place ship there!");
                return 0;
            }
        }
    }
    for(int x=loc.x;x>loc.x-len;x--)cur_b_st->board[x][loc.y]=id;
    return 1;
}
int b_v_E(Board_State* cur_b_st, Coords loc, int len,int id){
    Coords tmp_loc;
    for(int x=loc.x-1;x<=loc.x+len;x++){
        tmp_loc.x=x;
        for(int y=loc.y+1;y>=loc.y-1;y--){
            tmp_loc.y=y;
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0){
                printf("Can't place ship there!");
                return 0;
            }
        }
    }
    for(int x=loc.x;x<loc.x+len;x++)cur_b_st->board[x][loc.y]=id;
    return 1;
}
int board_verificator(Board_State* cur_b_st, Dir_coords new_boat, int len,int ship_id){
    switch (new_boat.dir)
    {
        case 'N':return b_v_N(cur_b_st,new_boat.c,len,ship_id);
        case 'S':return b_v_S(cur_b_st,new_boat.c,len,ship_id);
        case 'W':return b_v_W(cur_b_st,new_boat.c,len,ship_id);
        case 'E':return b_v_E(cur_b_st,new_boat.c,len,ship_id);
    }
}