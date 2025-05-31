#include <stdio.h>
#include "Btlshp.h"
#include <stdlib.h>

Board_State* board_player_setup();
void game_loop_2p(Board_State* p1,Board_State* p2);
void display_player_board(Board_State* B_S);
void display_opponent_board(Board_State* B_S);
int Ship_pos_verify(Ship_pos SP,int edit,int avail_2l,int avail_3l,int avail_4l,int avail_6l);

void main()
{
    int player_count=0;
    Board_State *player_1_board, *player_2_board;
    do{
       printf("Player count (1 or 2): "); 
       scanf("%d", &player_count);
    }while(player_count!=1&&player_count!=2);

    printf("\nPlayer 1 board setup:");
    //needs choice for load board file
    player_1_board=board_player_setup();
    
    if(player_count==2){
        printf("\nPlayer 2 board setup:");
        //needs choice for load board file
        player_2_board=board_player_setup();
        game_loop_2p(player_1_board,player_2_board);
    }else{
        player_2_board = NULL;//AI
    }
}

Board_State* board_player_setup(){
    Board_State* B_S = (Board_State*)malloc(sizeof(Board_State));
    int placed_ships=0,avail_2l=4,avail_3l=3,avail_4l=2,avail_6l=1;
    while(placed_ships <10){
        int ch;
        printf("\nOptions:\n 1.Place Ship\n 2.Remove Ship\n 3.Show Board\nChoose: ");
        scanf("%d", &ch);
        if(ch<1 || ch>3) printf("Invalid option!");
        else if(ch==3) display_player_board(B_S);
        else{
            Ship_pos sp;
            if(ch==2){
                do{
                    printf("\nEdit ship on X Y Dir Len: ");
                    scanf("%d %d %c %d",&sp.Ship_dir.c.x,&sp.Ship_dir.c.y,&sp.Ship_dir.dir,&sp.ship_len);
                }while(p_ship_request(sp,1,avail_2l,avail_3l,avail_4l,avail_6l));
                if(board_verificator(B_S,sp.Ship_dir,sp.ship_len*-1)){
                    placed_ships--;
                    switch(sp.ship_len){
                        case -2:avail_2l++;break;
                        case -3:avail_3l++;break;
                        case -4:avail_4l++;break;
                        case -6:avail_6l++;break;
                    }
                    do{
                        printf("\nPlace ship on X Y Dir: ");
                        scanf("%d %d %c",&sp.Ship_dir.c.x,&sp.Ship_dir.c.y,&sp.Ship_dir.dir);
                    }while(p_ship_request(sp,0,avail_2l,avail_3l,avail_4l,avail_6l));    
                }else printf("Can't find ship!");
            }else{
                do{
                    printf("\nPlace ship on X Y Dir Len: ");
                    scanf("%d %d %c %d",&sp.Ship_dir.c.x,&sp.Ship_dir.c.y,&sp.Ship_dir.dir,&sp.ship_len);
                }while(p_ship_request(sp,0,avail_2l,avail_3l,avail_4l,avail_6l));
            }
            if(board_verificator(B_S,sp.Ship_dir,sp.ship_len)){   
                placed_ships++;
                switch(sp.ship_len){
                    case 2:avail_2l--;break;
                    case 3:avail_3l--;break;
                    case 4:avail_4l--;break;
                    case 6:avail_6l--;break;
                }       
            }else printf("\nInvalid ship location!");            
        }
    }
    return B_S;
}

int in_board_range(int p){
    if(p<1||p>10) return 1;
    else 0;
}
int is_out_of_board(Ship_pos SP){
    switch (SP.Ship_dir.dir)
    {
        case 'N':SP.Ship_dir.c.y+=SP.ship_len;break;
        case 'S':SP.Ship_dir.c.y-=SP.ship_len;break;
        case 'W':SP.Ship_dir.c.x-=SP.ship_len;break;
        case 'E':SP.Ship_dir.c.x+=SP.ship_len;break;
    }
    if(in_board_range(SP.Ship_dir.c.x)&&in_board_range(SP.Ship_dir.c.y))return 0;
    else return 1;
}
int Ship_pos_verify(Ship_pos SP,int edit,int avail_2l,int avail_3l,int avail_4l,int avail_6l){
    switch (SP.Ship_dir.dir)
    {
        case 'N':SP.Ship_dir.dir=0;break;
        case 'S':SP.Ship_dir.dir=1;break;
        case 'W':SP.Ship_dir.dir=2;break;
        case 'E':SP.Ship_dir.dir=3;break;
    
        default:SP.Ship_dir.dir=-1;break;
    }
    if(!edit)switch(SP.ship_len){
        case 2:if(avail_2l==0) SP.ship_len=-1;break;
        case 3:if(avail_3l==0) SP.ship_len=-1;break;
        case 4:if(avail_4l==0) SP.ship_len=-1;break;
        case 6:if(avail_6l==0) SP.ship_len=-1;break;
        
        default:SP.ship_len=-1;break;
    }
    if(in_board_range(SP.Ship_dir.c.x)|| in_board_range(SP.Ship_dir.c.y)|| SP.Ship_dir.dir==-1||SP.ship_len==-1){
        printf("Bad Input Data!");
        return 0;
    }
    if(is_out_of_board(SP)){
        printf("Ship is out of board");
        return 0;
    }
    return 1;
}

void game_loop_2p(Board_State* p1,Board_State* p2){
    Board_State* boards[2]={p1,p2};
    Coords coords[2]={{0,0},{0,0}};
    int aim=0,target=1,temp=0;
    while(1){
        int ch=0;
        printf("\nPlayer %d turn:\n 1.Display opponent board\n 2.Fire\nChoose: ",aim+1);
        scanf("%d",&ch);
        if(ch==1) display_opponent_board(boards[target]);
        else if(ch==2){
            Coords c;
            do{
                printf("\nHow to aim:\n 1.Normal Fire\n 2.Take last coords\nChoose: ");
                scanf("%d",&ch);
                if(ch==1){
                    printf("\nX Y: ");
                    scanf("%d %d",&c.x,&c.y);
                    if(c.x<1||c.x>10||c.y<1||c.y>10)printf("Invalid coords!");
                    else{
                        ch=fire(boards[target],c);
                        //add fire atempt to replay
                        break;
                    }  
                }else if(ch==2){
                    char d;
                    c=coords[aim];
                    printf("\nDir: ");
                    scanf("%c",&d);
                    switch (d)
                    {
                        case 'N':c.y++;break;
                        case 'S':c.y--;break;
                        case 'W':c.x--;break;
                        case 'E':c.x++;break;
                    
                        default:ch=-1;break;
                    }
                    if(ch==-1)printf("Invalid direction!");
                    else if(c.x<1||c.x>10||c.y<1||c.y>10)printf("Invalid coords!");
                    else{
                        ch=fire(boards[target],c);
                        //add fire atempt to replay
                        break;
                    }  
                }else printf("Invalid option!");
            }while (1);
            if(ch==0)printf("Miss!");
            if(ch==1)printf("Hit!");
            if(ch==2)printf("Ship Destroyed");
            if(ch==3){printf("Game won by player %d!",aim+1);break;}
            coords[aim].x=c.x;
            coords[aim].y=c.y;
            temp=aim;
            aim=target;
            target=temp;
        }else printf("Invalid option!");
    }
    //Save and encrypt replay here
}

void display_player_board(Board_State* B_S){
    printf("My board");
}

void display_opponent_board(Board_State* B_S){
    printf("Oponent board");
}