#include <stdlib.h>
#include <stdio.h>

#define MAXHAND 40
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
    for (size = 0; arr[size] -> number != 255; size++); 
    for (int i = 0; i < size; i++)
        for (int j = 0; i < size - 1; j++)
            if (arr[j] -> number > arr[j + 1] -> number)
                tile_swap(arr[j], arr[j + 1]);
}

void fisher_yates(Tile* arr[]){        // Fisher Yates shuffle

	int randint;
    int size = 0;
    for (size = 0; (arr[size] -> number) != 255; size++);
	for (int i = (size - 1); i > 0; i--){
		randint = rand() % (i + 1);	
		tile_swap(arr[i], arr[randint]);
	} 
}
void pool_get(Tile *hand[], Tile* pool[]){

    Tile* voidTile;
    voidTile = malloc(sizeof(Tile));
    voidTile -> number = 255;
    voidTile -> suit = 255;

    int i, j;
    i = j = 0;
    
    /* Find the last tile on the hand (a void tile) */
    for (j = 0; hand[j] -> number != voidTile -> number; j++);

    (hand[j+1]) = voidTile;
    tile_swap(hand[j], pool[i]);

    /* pool[i] is now a void tile */
    /* this next loop brings the void tile to the end of the pool */ 
    while ((pool[i + 1] -> number) != (voidTile -> number)){
        tile_swap(pool[i], pool [i+1]);
        i++;
    } 

    free(pool[i+1]);
}

void arr_put(Tile* hand_tile, Tile* arr[]){
    
    

}

int main(int argc, char *argv[]){

    Tile* voidTile = malloc(sizeof(Tile));
    voidTile -> number = 255;
    voidTile -> suit = 255;

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

    pool[NUMOFTILES] = voidTile;
	
	Tile** players[2];
    players[0] = malloc(MAXHAND * sizeof(Tile));
    players[1] = malloc(MAXHAND * sizeof(Tile));

    players[0][0] = voidTile;
    players[1][0] = voidTile;

	fisher_yates(pool);

    Tile table[NUMOFTILES];


    for (int i = 0; pool[i] -> number != voidTile -> number; i++){
        printf("Num %d, Suit: %d\n", pool[i] -> number, pool[i] -> suit);
    }
    
    for (int i = 0; i < 14; i++){
        
    }

}
