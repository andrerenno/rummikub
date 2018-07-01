#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#if defined(_WIN32) || defined(WIN32)
#define clearBuffer() fflush(stdin)
#define clearScreen() system("cls")
#else
#include <stdio_ext.h>
#define clearBuffer() __fpurge(stdin)
#define clearScreen() system("clear")
#endif


#define NUMOFTILES 106
#define MAXHAND 106


typedef struct
{
    unsigned char number;
    unsigned char suit;
} Tile;


Tile **hand; /* array bidimensional onde o primeiro index corresponde ao
                player, e o segundo index corresponde às cartas da mão */


//colocar os prototipos das funcoes aqui
void firstRound(int num);
void handPlayer(int num);
void pool();


int main()
{
    int nPlayers; //qtd de jogadores

    printf("Informe a quantidade de jogadores (1-5): ");
    clearBuffer();
    scanf("%d", &nPlayers);

   hand = (Tile **) malloc(sizeof(nPlayers));
    if (hand != NULL)
    {
        for (int i = 0; i < nPlayers; i++)
        {
            hand[i] = (Tile *) malloc(sizeof(Tile));
        }
    }

    firstRound(nPlayers);
    handPlayer(nPlayers);

    printf("carta: %c%c\n", hand[0][0].number, hand[0][0].suit);

    return 0;
}


void firstRound(int num) //num == nPlayers
{
    int indexN, indexS; //index para os vetores de numeros e de naipes
    char vetorN[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D'}; //numeros validos
    char vetorS[] = {'!', '@', '#', '$'}; //naipes validos

    /*fornecendo uma carta aleatoria para cada jogador*/
    srand((unsigned) time(NULL));
    for (int i = 0; i < num; i++)
    {
        indexN = rand() % 13;
        indexS = rand() % 4;

        hand[i][0].number = vetorN[indexN];
        hand[i][0].suit = vetorS[indexS];
    }
}


void handPlayer(int num) //num == nPlayers
{
    int indexN, indexS; //index para os vetores de numeros e de naipes
    char vetorN[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D'}; //numeros validos
    char vetorS[] = {'!', '@', '#', '$'}; //naipes validos

    for (int i = 0; i < num; i++)
    {
        hand[i] = realloc(hand, 14);
        if (!hand)
        {
            printf("Erro na distribuicao das cartas.\n");
            clearBuffer();
            getchar();
            exit(1);
        }
        /*fornecendo 14 cartas aleatorias a cada jogador*/
        for (int j = 0; j < 14; j++)
        {
            indexN = rand() % 13;
            indexS = rand() % 4;

            hand[i][j].number = vetorN[indexN];
            hand[i][j].suit = vetorS[indexS];
        }
    }

}


void pool() //montante
{

}
