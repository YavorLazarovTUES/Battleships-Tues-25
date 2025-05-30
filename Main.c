#include <stdio.h>
#include "Btlshp.h"
#include <stdlib.h>

Board_State* board_player_setup();
void game_loop_2p(Board_State* p1,Board_State* p2);
void display_player_board(Board_State* B_S);
void display_opponent_board(Board_State* B_S);

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
            Dir_coords d_c;
            int len;
            char dir;
            printf("\nX Y Dir Len: ");
            scanf("%d %d %c %d",&d_c.c.x,&d_c.c.y,&dir,&len);
            switch (dir)
            {
                case 'N':d_c.dir=0;break;
                case 'S':d_c.dir=1;break;
                case 'W':d_c.dir=2;break;
                case 'E':d_c.dir=3;break;
            
                default:d_c.dir=-1;break;
            }
            switch(len){
                case 2:if(avail_2l==0) len=-1;break;
                case 3:if(avail_3l==0) len=-1;break;
                case 4:if(avail_4l==0) len=-1;break;
                case 6:if(avail_6l==0) len=-1;break;
                
                default:len=-1;break;
            }
            if(d_c.c.x<1||d_c.c.x>10 || d_c.c.y<1||d_c.c.y>10 || d_c.dir==-1||len==-1) printf("\nBad input data!");
            else if(ch==2)len*=-1;

            if(board_verificator(B_S,d_c,len)){
                if(ch==1){
                    placed_ships++;
                    switch(len){
                        case 2:avail_2l--;break;
                        case 3:avail_3l--;break;
                        case 4:avail_4l--;break;
                        case 6:avail_6l--;break;
                    }
                }else{
                    placed_ships--;
                    switch(len){
                        case -2:avail_2l++;break;
                        case -3:avail_3l++;break;
                        case -4:avail_4l++;break;
                        case -6:avail_6l++;break;
                    }
                }
            }else{
                if(ch==1)printf("\nInvalid ship location!");
                else printf("Can't find ship!");
            }
        }
    }
    return B_S;
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