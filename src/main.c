#include <stdlib.h>

#include <stdio.h>
#include <curses.h>

#define MAXHAND 106
#define NUMOFTILES 106


typedef struct Tile {
    unsigned char number;
    unsigned char suit;
} Tile;

void tile_swap(Tile *a1, Tile *a2){
    Tile temp = *a1;
    *a1 = *a2;
    *a2 = temp;
}

void tile_sort(Tile* arr[]){

    int size;
    for (size = 0; arr[size] != NULL; size++); 
    for (int i = 0; i < size; i++)
        for (int j = 0; i < size - 1; j++)
            if (arr[j] -> number > arr[j + 1] -> number)
                tile_swap(arr[j], arr[j + 1]);
}

void fisher_yates(Tile* arr[]){        // Fisher Yates shuffle

    int randint;
    int size = 0;
    for (size = 0; arr[size] != NULL; size++);
    for (int i = (size - 1); i > 0; i--){
        randint = rand() % (i + 1);	
        tile_swap(arr[i], arr[randint]);
    } 
}
void pool_get(Tile *hand[], Tile* pool[]){

    int hand_pos, pool_pos;
    hand_pos = pool_pos = 0;

    /* Find the last element on the hand array (a NULL pointer) */
    for (hand_pos = 0; hand[hand_pos] != NULL; hand_pos++);
    
    /* Checks if the pool is empty */
    if(pool[0] == NULL)
        return;

    hand[hand_pos+1] = NULL;
    hand[hand_pos] = pool[0];

    /* this next loop brings the tile that the player got to the end of the array (after the NULL pointer) */ 
    while (pool[pool_pos-1] != NULL ){
        pool[pool_pos] = pool[pool_pos + 1];
        pool_pos++;
    } 

}

void arr_put(Tile* hand[], unsigned int index, Tile* arr[]){

    /* Test if 'index' is a valid position on the array */
    int hand_size;
    for (hand_size = 0; hand[hand_size] != NULL);

    if (index > hand_size)
        return;


    int i;

    for(i = 0; arr[i] != NULL; i++);
    arr[i + 1] = NULL;
    arr[i] = hand[index];

    while (hand[index-1] != NULL ){
        hand[index] = hand[index + 1];
        index++;
    } 

}

int main(int argc, char *argv[]){
	/* curses initializations. */
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, FALSE);
	curs_set(0);
	start_color();
	init_pair(1, COLOR_MAGENTA, COLOR_BLACK);	//Used for the "Quarto" in the title screen.
	init_pair(2, COLOR_BLUE, COLOR_BLACK);  	//Used for player 1's turn.
	init_pair(3, COLOR_GREEN, COLOR_BLACK);		//Used for player 2's turn.
	init_pair(4, COLOR_WHITE, COLOR_RED);		//Used for error messages.
	init_pair(5, COLOR_CYAN, COLOR_BLACK);		//Used for the X's that replace numbers that where already used. 



    Tile** pool = malloc((NUMOFTILES + 2) * sizeof(Tile*));
    if (pool == NULL)
        return 1;

    for (int i = 0; i < (NUMOFTILES - 2); i++){
        pool[i] = malloc(sizeof(Tile));
        pool[i] -> number = (i % 13) + 1;
        pool[i] -> suit = (i / 13) % 4;
    }

    pool[NUMOFTILES - 1] = malloc(sizeof(Tile));
    pool[NUMOFTILES - 1] -> number = 14;
    pool[NUMOFTILES - 1] -> suit = 2;

    pool[NUMOFTILES - 2] = malloc(sizeof(Tile));
    pool[NUMOFTILES - 2] -> number = 14;
    pool[NUMOFTILES - 2] -> suit = 1;

    pool[NUMOFTILES] = NULL;

    Tile** player[2];
    player[0] = malloc(MAXHAND * sizeof(Tile));
    player[1] = malloc(MAXHAND * sizeof(Tile));

    player[0][0] = NULL;
    player[1][0] = NULL;

    fisher_yates(pool);

    Tile table[NUMOFTILES];

    int c = 0;
    while ((c = getch()) != '\n'){
        pool_get(player[0], pool);

        clear();
        printw("Player Hand\n");
        for (int i = 0; player[0][i] != NULL ; i++){
            printw("Num %d, Suit: %d\n", player[0][i] -> number, player[0][i] -> suit);
        }
    }

    endwin();

}
