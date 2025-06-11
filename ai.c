#include "ai.h"

Board_State* generate_ai_board() {
    Board_State* B_S = (Board_State*)malloc(sizeof(Board_State));
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            B_S->board[i][j] = 0;
        }
    }
    
    int ship_lengths[] = {2, 2, 2, 2, 3, 3, 3, 4, 4, 6};
    int ship_id = 1;
    char ch;
    
    for(int i = 0; i < 10; i++) {
        int placed = 0;
        int attempts = 0;
        while(!placed && attempts < 1000) {
            Ship_pos sp;
            sp.ship_len = ship_lengths[i];
            sp.Ship_dir.c.x = rand() % 10;
            sp.Ship_dir.c.y = rand() % 10;
            
            int dir = rand() % 4;
            switch(dir) {
                case 0: sp.Ship_dir.dir = 'N'; break;
                case 1: sp.Ship_dir.dir = 'S'; break;
                case 2: sp.Ship_dir.dir = 'W'; break;
                case 3: sp.Ship_dir.dir = 'E'; break;
            }
            
            if(!is_ship_out_of_board(sp)) {
                if(board_verificator(B_S, sp.Ship_dir, sp.ship_len, ship_id)) {
                    placed = 1;
                    ship_id++;
                }
            }
            attempts++;
        }
        
        if(!placed) {
            free(B_S);
            return generate_ai_board();
        }
    }
    
    return B_S;
}

AI_State init_ai_state() {
    AI_State state = {0, {-1, -1}, {-1, -1}, 'N', 0};
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            state.moves[i][j] = 0;
        }
    }
    return state;
}

Coords get_ai_target(Board_State* player_board, AI_State* ai_state) {
    Coords target;
    int attempts = 0;
    int max_attempts = 100; 
    
    if(ai_state->state == 0) {
        do {
            target.x = rand() % 10;
            target.y = rand() % 10;
            attempts++;
            if (attempts > max_attempts) {
                ai_state->state = 0;
                for(int i = 0; i < 10; i++) {
                    for(int j = 0; j < 10; j++) {
                        ai_state->moves[i][j] = 0;
                    }
                }
                target.x = rand() % 10;
                target.y = rand() % 10;
                break;
            }
        } while(ai_state->moves[target.y][target.x] == 1); 
    }
    else if(ai_state->state == 1) {
        static int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // N, E, S, W
        static int dir_index = 0;
        int start_dir = dir_index;
        do {
            target.x = ai_state->first_hit.x + directions[dir_index][0];
            target.y = ai_state->first_hit.y + directions[dir_index][1];
            attempts++;
            
            if(target.x >= 0 && target.x <= 9 && target.y >= 0 && target.y <= 9 && ai_state->moves[target.y][target.x] == 0) {
                break;
            }
            
            dir_index = (dir_index + 1) % 4;
            if(dir_index == start_dir) {
                ai_state->state = 0;
                return get_ai_target(player_board, ai_state);
            }
            
            if (attempts > max_attempts) {
                ai_state->state = 0;
                return get_ai_target(player_board, ai_state);
            }
        } while(1);
    }
    else if(ai_state->state == 2) {
        target = ai_state->last_hit;
        int dx = 0, dy = 0;
        
        int dir_dx = ai_state->last_hit.x - ai_state->first_hit.x;
        int dir_dy = ai_state->last_hit.y - ai_state->first_hit.y;
        
        if(dir_dx != 0) {
            ai_state->direction = dir_dx > 0 ? 'E' : 'W';
        } else if(dir_dy != 0) {
            ai_state->direction = dir_dy > 0 ? 'S' : 'N';
        }
        
        switch(ai_state->direction) {
            case 'N': dy = -1; break;
            case 'S': dy = 1; break;
            case 'W': dx = -1; break;
            case 'E': dx = 1; break;
        }
        if(ai_state->reverse) {
            dx = -dx;
            dy = -dy;
            target = ai_state->first_hit;
        }
        target.x += dx;
        target.y += dy;
        
        if(target.x < 0 || target.x > 9 || target.y < 0 || target.y > 9 || ai_state->moves[target.y][target.x] == 1) {
            if(!ai_state->reverse) {
                ai_state->reverse = 1;
                target = ai_state->first_hit;
                target.x -= dx;
                target.y -= dy;
            } else {
                ai_state->state = 1; 
                ai_state->reverse = 0;
                return get_ai_target(player_board, ai_state);
            }
        }
    }
    
    if (target.x < 0 || target.x > 9 || target.y < 0 || target.y > 9) {
        ai_state->state = 0;
        return get_ai_target(player_board, ai_state);
    }
    
    ai_state->moves[target.y][target.x] = 1;
    return target;
}

void update_ai_state(AI_State* ai_state, Coords target, int hit_result) {
    if(hit_result == 1) { // Hit
        if(ai_state->state == 0) {
            ai_state->state = 1;
            ai_state->first_hit = target;
            ai_state->last_hit = target;
        }
        else if(ai_state->state == 1) {
            // direction
            ai_state->state = 2;
            ai_state->last_hit = target;
            
            int dx = target.x - ai_state->first_hit.x;
            int dy = target.y - ai_state->first_hit.y;
            
            if(dx != 0) {
                ai_state->direction = dx > 0 ? 'E' : 'W';
            } else if(dy != 0) {
                ai_state->direction = dy > 0 ? 'S' : 'N';
            }
        }
        else if(ai_state->state == 2) {
            // continue the same direction
            ai_state->last_hit = target;
        }
    }
    else if(hit_result == 0) { // Miss
        if(ai_state->state == 2) {
            // opposite direction if miss
            if(!ai_state->reverse) {
                ai_state->reverse = 1;
                ai_state->last_hit = ai_state->first_hit;
            } else {
                ai_state->state = 1;
                ai_state->reverse = 0;
            }
        }
    }
    else if(hit_result == 2) { //destroyed
        ai_state->state = 0;
        ai_state->reverse = 0;
        ai_state->first_hit.x = -1;
        ai_state->first_hit.y = -1;
        ai_state->last_hit.x = -1;
        ai_state->last_hit.y = -1;
        ai_state->direction = 'N';
    }
}

void game_loop_1p(Board_State* player_board, Board_State* ai_board) {
    AI_State ai_state = init_ai_state();
    Coords player_last_coords = {-1, 0};
    int game_over = 0;
    int player_turn = 1; 
    
    while(!game_over) {
        if(player_turn) {
            int ch=0;
            printf("\nYour turn:\n 1.Display opponent board\n 2.Fire\nChoose: ");
            scanf("%d",&ch);
            if(ch==1) display_opponent_board(ai_board);
            else if(ch==2){
                Coords c;
                do{ 
                    if(player_last_coords.x==-1){
                        printf("\nX Y: ");
                        scanf("%d %d",&c.x,&c.y);
                        c.x--;
                        c.y--;
                        if(c.x<0||c.x>9||c.y<0||c.y>9)printf("Invalid coords!");
                        else{
                            ch=fire(ai_board,c);
                            Writecoords(filename,0,c.x, c.y);
                            break;
                        }
                    }
                    else{
                        printf("\nHow to aim:\n 1.Normal Fire\n 2.Take last coords (%02d;%02d)\nChoose: ",player_last_coords.x+1,player_last_coords.y+1);
                        scanf("%d",&ch);
                        if(ch==1){
                            printf("\nX Y: ");
                            scanf("%d %d",&c.x,&c.y);
                            c.x--;
                            c.y--;
                            if(c.x<0||c.x>9||c.y<0||c.y>9)printf("Invalid coords!");
                            else{
                                Writecoords(filename,0,c.x, c.y);
                                ch=fire(ai_board,c);
                                break;
                            }  
                        }else if(ch==2){
                            char d;
                            c=player_last_coords;
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
                                Writecoords(filename,0,c.x, c.y);
                                ch=fire(ai_board,c);
                                break;
                            }  
                        }else printf("Invalid option!");
                    }
                }while (1);
                player_last_coords.x=c.x;
                player_last_coords.y=c.y;
                if(ch==1)printf("Hit!");
                if(ch==2)printf("Ship Destroyed!");
                if(ch==3){
                    printf("You won!");game_over=1;
                    do{
                    printf("\n Would you like to see a replay of the game? (Y/N): ");
                    scanf(" %c", &ch);
                }while(ch!='Y'&& ch!='y'&& ch!='N'&& ch!='n');
                if(ch=='Y'||ch=='y') playReplay(filename);
                else if(ch=='N'||ch=='n') printf("Ok");
                }
                if(ch==0){
                    printf("Miss!");
                    player_turn=0;
                }
            }else printf("Invalid option!");
        } else {
            // ai turn
            Coords ai_target = get_ai_target(player_board, &ai_state);
            printf("\nAI fires at (%d,%d)\n", ai_target.x + 1, ai_target.y + 1);
            int result = fire(player_board, ai_target);
            Writecoords(filename,1,ai_target.x, ai_target.y);
            update_ai_state(&ai_state, ai_target, result);
            
            // result
            if(result == 0) {
                printf("AI Missed!");
                player_turn = 1; //player's turn
                printf("\n----------------------------------------\n");
            }
            else if(result == 1) {
                printf("AI Hit!");
                // AI continues its turn
            }
            else if(result == 2) {
                printf("AI Destroyed a Ship!");
            }
            else if(result == 3) {
                printf("\nAI won!");
                game_over = 1;
            }
            
            printf("\nYour board after AI's move:\n");
            display_player_board(player_board);
            
            if(result == 0) {
                player_turn = 1;
            }
        }
    }
}