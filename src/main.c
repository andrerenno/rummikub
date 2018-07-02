#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio_ext.h>

#define MAXHAND 106
#define NUMOFTILES 106


void filePool(unsigned int num);


typedef struct {
    char number;
    char suit;
} Tile;


int main()
{
    unsigned int nPlayers;

    printf("Informe a quantidade de jogadores (1-5): ");
    __fpurge(stdin);
    scanf("%ud", &nPlayers);

    filePool(nPlayers);

    return 0;
}


void filePool(unsigned int num)
{
    FILE *fp;
    Tile *pool = (Tile *) malloc(NUMOFTILES * sizeof(Tile));

    char charN[2]; //le o numero
    char charS[2]; //le o naipe
    char enter[2]; //le o \n

    fp = fopen("baralho.txt", "r");

    for (int i = 0; i < NUMOFTILES; i++)
    {
        fgets(charN, sizeof(charN), fp);
        fgets(charS, sizeof(charS), fp);
        fgets(enter, sizeof(enter), fp);

        pool[i].number = charN[0];
        pool[i].suit = charS[0];
    }

    fclose(fp);
}
