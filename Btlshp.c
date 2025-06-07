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
    for(int i=0;i<k;i++)tgt_b_st->board[ship_det[i].x][ship_det[i].y]=trgt+20;
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
    if(loc.x==-1||loc.x==10||loc.y==-1||loc.y==10) return 0;
    return cur_b_st->board[loc.x][loc.y];
}
int b_v_N(Board_State* cur_b_st, Coords loc, int len,int id){
    Coords tmp_loc;
    for(int x=loc.x-1;x<=loc.x+1;x++){
        tmp_loc.x=x;
        for(int y=loc.y-len;y<=loc.y+1;y++){
            tmp_loc.y=y;
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0)return 0;
        }
    }
    for(int y=loc.y;y>loc.y-len;y--)cur_b_st->board[loc.x][y]=id;
    return 1;
}
int b_v_S(Board_State* cur_b_st, Coords loc, int len,int id){
    Coords tmp_loc;
    for(int x=loc.x-1;x<=loc.x+1;x++){
        tmp_loc.x=x;
        for(int y=loc.y-1;y<=loc.y+len;y++){
            tmp_loc.y=y;
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0)return 0;
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
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0)return 0;
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
            if(b_v_check_coords(cur_b_st,tmp_loc)!=0)return 0;
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


void display_player_board(Board_State* B_S){
    printf("\nMy board:\n");
    printf("y x");
    for(int i=0;i<10;i++)printf("%02d ",i+1);
    printf("\n");
    uint8_t val;
    for(int i=0;i<10;i++){
        printf("%02d ",i+1);
        for(int j=0;j<10;j++){
            val=B_S->board[j][i];
            switch (val)
            {
            case 0: printf("~~ ");break;
            case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
            case 8: case 9: case 10: printf("%02d ",val);break;
            case 11: case 12: case 13: case 14: case 15: case 16: case 17: 
            case 18: case 19: case 20: printf("HH ");break;
            case 21: case 22: case 23: case 24: case 25: case 26: case 27: 
            case 28: case 29: case 30: printf("DD ");break;
            case 32: printf("MM ");break;
            default: printf("~~ ");break;
            }
        }
        printf("\n");
    }
}

void display_opponent_board(Board_State* B_S){
    printf("Oponent board:\n");
    printf("y x");
    for(int i=0;i<10;i++)printf("%02d ",i+1);
    printf("\n");
    uint8_t val;
    for(int i=0;i<10;i++){
        printf("%02d ",i+1);
        for(int j=0;j<10;j++){
            val=B_S->board[j][i];
            switch (val)
            {
            case 11: case 12: case 13: case 14: case 15: case 16: case 17: 
            case 18: case 19: case 20: printf("HH "); break;
            case 21: case 22: case 23: case 24: case 25: case 26: case 27: 
            case 28: case 29: case 30: printf("DD "); break;
            case 32: printf("MM ");break;
            default: printf("~~ ");break;
            }
        }
        printf("\n");
    }
}

int is_ship_out_of_board(Ship_pos SP){
    switch (SP.Ship_dir.dir)
    {
        case 'N':SP.Ship_dir.c.y-=SP.ship_len-1;break;
        case 'S':SP.Ship_dir.c.y+=SP.ship_len-1;break;
        case 'W':SP.Ship_dir.c.x-=SP.ship_len-1;break;
        case 'E':SP.Ship_dir.c.x+=SP.ship_len-1;break;
    }
    if(out_of_board_range(SP.Ship_dir.c.x)||out_of_board_range(SP.Ship_dir.c.y))return 1;
    else return 0;
}

int out_of_board_range(int p){
    return (p<0||p>9);
}