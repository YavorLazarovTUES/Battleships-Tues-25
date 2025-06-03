#include <stdio.h>
#include "Btlshp.h"
#include <stdlib.h>
#include <string.h>

Board_State* board_player_setup();
Board_State* board_file_setup();
void game_loop_2p(Board_State* p1,Board_State* p2);
void display_player_board(Board_State* B_S);
void display_opponent_board(Board_State* B_S);
int Ship_pos_verify(Ship_pos SP,int edit,int *id_avail);
void Add_ship(Board_State* B_S,int *id_avail);
void Edit_ship(Board_State* B_S,int *id_avail);

void main()
{
    int player_count=0;
    char ch;
    Board_State *player_1_board, *player_2_board;
    do{
       printf("Player count (1 or 2): "); 
       scanf("%d", &player_count);
    }while(player_count!=1&&player_count!=2);
    getchar();
    printf("\nPlayer 1 board setup:");
    do{
        printf("\n Load file (Y/N): ");
        scanf("%c",&ch);
    }while(ch!='Y'&& ch!='N');
    if(ch=='Y') player_1_board=board_file_setup();
    else if(ch=='N') player_1_board=board_player_setup();
    
    if(player_count==2){
        printf("\nPlayer 2 board setup:");
        do{
            printf("\n Load file (Y/N): ");
            scanf("%c",&ch);
        }while(ch!='Y'&& ch!='N');
        if(ch=='Y') player_2_board=board_file_setup();
        else if(ch=='N') player_2_board=board_player_setup();
        game_loop_2p(player_1_board,player_2_board);
    }else{
        player_2_board = NULL;//AI
    }
}

int out_of_board_range(int p){
    return (p<0||p>9);
}

Board_State* board_player_setup(){
    Board_State* B_S = (Board_State*)malloc(sizeof(Board_State));
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            B_S->board[i][j]=0;
        }
    }
    int id_avail[10];//0-3 2l ;4-6 3l; 7-8 4l; 9 6l;
    for (int i = 0; i < 10; i++) id_avail[i] = 1;

    for(int placed_ships=0;placed_ships<10;){
        if(placed_ships==0)Add_ship(B_S,id_avail),placed_ships++;
        int ch;
        printf("\nOptions:\n 1.Place Ship\n 2.Edit Ship\n 3.Show Board\nChoose: ");
        scanf("%d", &ch);
        switch (ch)
        {
            case 1:Add_ship(B_S,id_avail);placed_ships++;break;
            case 2:Edit_ship(B_S,id_avail);break;
            case 3:display_player_board(B_S);break;
            
            default:printf("Invalid option!");break;
        } 
        
    }
    return B_S;
}
Board_State* board_file_setup(){
    Board_State* B_S = (Board_State*)malloc(sizeof(Board_State));
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            B_S->board[i][j]=0;
        }
    }
    int id_avail[10];//0-3 2l ;4-6 3l; 7-8 4l; 9 6l;
    for (int i = 0; i < 10; i++) id_avail[i] = 1;
    char file_name[100];
    getchar();
    printf("\nGive file name: ");
    gets(file_name);
    Ship_pos sp;
    int c;
    FILE* f=fopen(file_name,"r");
    if (f == NULL) {
       printf("Error opening file!");
       exit(1);
    }
    for (int i = 0; i < 10; i++){
        fscanf(f,"%d %d %c %d",&sp.Ship_dir.c.x,&sp.Ship_dir.c.y,&sp.Ship_dir.dir,&sp.ship_len);
        sp.Ship_dir.c.x--;
        sp.Ship_dir.c.y--;
        c=Ship_pos_verify(sp,0,id_avail);
        if(c==0){printf("Error with file!");exit(1);}
        if(!board_verificator(B_S,sp.Ship_dir,sp.ship_len,c)){printf("Error with file!");exit(1);}
        id_avail[c-1]=0;
    }
    fclose(f);
    display_player_board(B_S);
    return B_S;
}
void Add_ship(Board_State* B_S,int* id_avail){
    Ship_pos sp;
    int c;
    do{     
        do{
            printf("\nPlace ship on X Y Dir Len: ");
            scanf("%d %d %c %d",&sp.Ship_dir.c.x,&sp.Ship_dir.c.y,&sp.Ship_dir.dir,&sp.ship_len);
            sp.Ship_dir.c.x--;
            sp.Ship_dir.c.y--;
            c=Ship_pos_verify(sp,0,id_avail);
        }while(c==0);
    }while(!board_verificator(B_S,sp.Ship_dir,sp.ship_len,c));
    id_avail[c-1]=0;
}
void Edit_ship(Board_State* B_S,int *id_avail){
    int rm_s;
    do{
      printf("\nID of ship to edit:");
      scanf("%d",&rm_s);
    }while (out_of_board_range(rm_s-1)||id_avail[rm_s-1]==1);
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(B_S->board[i][j]==rm_s)B_S->board[i][j]=0;
        }
    }
    Ship_pos sp;
    switch(rm_s)
    {
        case 1: case 2: case 3: case 4: sp.ship_len=2; break;
        case 5: case 6: case 7: sp.ship_len=3; break;
        case 8: case 9: sp.ship_len=4; break;
        case 10: sp.ship_len=6; break;
    }
    do{     
        do{
            printf("\nPlace ship on X Y Dir: ");
            scanf("%d %d %c",&sp.Ship_dir.c.x,&sp.Ship_dir.c.y,&sp.Ship_dir.dir);
            sp.Ship_dir.c.x--;
            sp.Ship_dir.c.y--;
        }while(!Ship_pos_verify(sp,1,id_avail));
    }while(!board_verificator(B_S,sp.Ship_dir,sp.ship_len,rm_s));
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
int Ship_pos_verify(Ship_pos SP,int edit,int *id_avail){
    int id_out=-1;
    switch (SP.Ship_dir.dir)
    {
        case 'N':break;
        case 'S':break;
        case 'W':break;
        case 'E':break;
    
        default:SP.Ship_dir.dir=0;break;
    }
    if(!edit)switch(SP.ship_len){
        case 2:for(int i=0;i<4;i++){
            if(id_avail[i]==1){
                id_out=i+1;
                break;
            }
        }break;
        case 3:for(int i=4;i<7;i++){
            if(id_avail[i]==1){
                id_out=i+1;
                break;
            }
        }break;
        case 4:for(int i=7;i<9;i++){
            if(id_avail[i]==1){
                id_out=i+1;
                break;
            }
        }break;
        case 6:if(id_avail[9]==1)id_out=10;break;
        default:SP.ship_len=-1;break;
    }
    if(out_of_board_range(SP.Ship_dir.c.x)|| out_of_board_range(SP.Ship_dir.c.y)|| SP.Ship_dir.dir==0||SP.ship_len==-1){
        printf("Bad Input Data!");
        return 0;
    }
    if(is_ship_out_of_board(SP)){
        printf("Ship is out of board");
        return 0;
    }
    if(!edit)if(id_out==-1){
        printf("No available ships of that size!");
        return 0;
    }else return id_out;
    return 1;
}

void game_loop_2p(Board_State* p1,Board_State* p2){
    Board_State* boards[2]={p1,p2};
    Coords coords[2]={{-1,0},{-1,0}};
    int aim=0,target=1,temp=0;
    while(1){
        int ch=0;
        printf("\nPlayer %d turn:\n 1.Display opponent board\n 2.Fire\nChoose: ",aim+1);
        scanf("%d",&ch);
        if(ch==1) display_opponent_board(boards[target]);
        else if(ch==2){
            Coords c;
            do{ 
                if(coords[aim].x==-1){
                    printf("\nX Y: ");
                    scanf("%d %d",&c.x,&c.y);
                    c.x--;
                    c.y--;
                    if(c.x<0||c.x>9||c.y<0||c.y>9)printf("Invalid coords!");
                    else{
                        ch=fire(boards[target],c);
                        //add fire atempt to replay
                        break;
                    }
                }
                else{
                    printf("\nHow to aim:\n 1.Normal Fire\n 2.Take last coords (%02d;%02d)\nChoose: ",coords[aim].x+1,coords[aim].y+1);
                    scanf("%d",&ch);
                    if(ch==1){
                        printf("\nX Y: ");
                        scanf("%d %d",&c.x,&c.y);
                        c.x--;
                        c.y--;
                        if(c.x<0||c.x>9||c.y<0||c.y>9)printf("Invalid coords!");
                        else{
                            ch=fire(boards[target],c);
                            //add fire atempt to replay
                            break;
                        }  
                    }else if(ch==2){
                        char d;
                        c=coords[aim];
                        getchar();
                        printf("\nDir: ");
                        scanf("%c",&d);
                        switch (d)
                        {
                            case 'N':c.y--;break;
                            case 'S':c.y++;break;
                            case 'W':c.x--;break;
                            case 'E':c.x++;break;
                        
                            default:ch=-1;break;
                        }
                        if(ch==-1)printf("Invalid direction!");
                        else if(c.x<0||c.x>9||c.y<0||c.y>9)printf("Invalid coords!");
                        else{
                            ch=fire(boards[target],c);
                            //add fire atempt to replay
                            break;
                        }  
                    }else printf("Invalid option!");
                }
            }while (1);
            coords[aim].x=c.x;
            coords[aim].y=c.y;
            if(ch==1)printf("Hit!");
            if(ch==2)printf("Ship Destroyed!");
            if(ch==3){printf("Game won by player %d!",aim+1);break;}
            if(ch==0){
                printf("Miss!");
                temp=aim;
                aim=target;
                target=temp;
            }
        }else printf("Invalid option!");
    }
    //Save and encrypt replay here
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
            case 8: case 9: case 10: printf("%02d ",val);
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
            case 18: case 19: case 20: printf("HH ",val); break;
            case 21: case 22: case 23: case 24: case 25: case 26: case 27: 
            case 28: case 29: case 30: printf("DD ",val); break;
            case 32: printf("MM ");break;
            default: printf("~~ ");break;
            }
        }
        printf("\n");
    }
}