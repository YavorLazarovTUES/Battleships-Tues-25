#include <stdio.h>
#include "replay.h"
#include "SDES.h"

void main()
{
    uint16_t key ; 
    char filename[100];
    FILE *f;
    do{
        printf("Name of game replay: ");
        scanf("%s",filename);
         f= fopen(filename, "r");
    }while(!f);
    fclose(f);
    do{
        printf("\n Key for encryption (<1023): ");
        scanf("%hu", &key);
    }while(key>1023);
    sdes_dec(filename,key);
    playReplay("decrypted.txt");
}