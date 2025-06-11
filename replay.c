#include <stdio.h>
#include <stdlib.h>
#include "Btlshp.h"
#include <stdint.h>
#include<unistd.h>
FILE *file;
void Write(char filename[100], uint8_t player_1_replay[10][10],uint8_t player_2_replay[10][10]){
 file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file");
        return;
    }
for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fprintf(file, "%d ", player_1_replay[i][j]);
        }
        fprintf(file, "\n");
    }

for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fprintf(file, "%d ", player_2_replay[i][j]);
        }
        fprintf(file, "\n");
    }
fclose(file);
}
void Writecoords(char filename[100],int target, int x, int y){
    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file");
        return;
    }
    fseek(file, 0, SEEK_END);
    fprintf(file, "%d %d %d\n", target, x, y);
    fclose(file);
}
void playReplay(char filename[100]){
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file");
        return;
    }
    Board_State board[2];
    Coords c;
for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
        fscanf(file, "%d", &board[0].board[i][j]);
        if(board[0].board[i][j]<0||board[0].board[i][j]>10){
            fclose(file);
            printf("Wrong key for encryption");
            return;
        }
    }
}
for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
        fscanf(file, "%d", &board[1].board[i][j]);
        if(board[0].board[i][j]<0||board[0].board[i][j]>10){
            fclose(file);
            printf("Wrong key for encryption");
            return;
        }
    }
}
display_player_board(&board[0]);
display_player_board(&board[1]);
while(1){
    int target;
    if(feof(file)) break;
    sleep(5);
    fscanf(file, "%d %d %d", &target, &c.x, &c.y);
    fire(&board[target], c);
    display_opponent_board(&board[target]);
}
fclose(file);
}
    