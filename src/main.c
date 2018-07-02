#include <stdlib.h>

#include <stdio.h>
#include <curses.h>
#include <time.h>

#define MAXHAND 106
#define NUMOFTILES 106


typedef struct Tile {
    unsigned char number;
    char suit;
} Tile;

void tile_swap(Tile *a1, Tile *a2){
    Tile temp = *a1;
    *a1 = *a2;
    *a2 = temp;
}


void tile_sort(Tile**);
void fisher_yates(Tile**);        // Fisher Yates shuffle
void pool_get(Tile**, Tile**);
int check_table(Tile***);
int check_run (Tile**);
int check_group (Tile**);
void arr_put(Tile**, unsigned int, Tile**);
Tile** new_set(void);
Tile** make_pool(int);

void tile_sort(Tile* arr[]){

    int size;
    for (size = 0; arr[size] != NULL; size++); 
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size - 1; j++)
            if (arr[j] -> number > arr[j + 1] -> number){ 
                Tile* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j+1] = temp;
            }
}

void fisher_yates(Tile* arr[]){        // Fisher Yates shuffle

    int randint;
    int size = 0;
    srand(time(NULL));
    for (size = 0; arr[size] != NULL; size++);
    for (int i = (size - 1); i > 0; i--){
        randint = rand() % (i + 1);	
        tile_swap(arr[i], arr[randint]);
    } 
}
void pool_get(Tile* hand[], Tile* pool[]){

    int hand_pos, pool_pos;
    hand_pos = pool_pos = 0;

    /* Find the last element on the hand array (a NULL pointer) */
    for (hand_pos = 0; hand[hand_pos] != NULL; hand_pos++);
    
    /* Checks if the pool is empty */
    if(pool[0] == NULL)
        return;

    hand[hand_pos+1] = NULL;
    hand[hand_pos] = pool[0];

    /* this next shifts the array on the right side of the tile to the left  */ 
    while (pool[pool_pos] != NULL ){
        pool[pool_pos] = pool[pool_pos + 1];
        pool_pos++;
    } 

}

void arr_put(Tile* hand[], unsigned int index, Tile* arr[]){

    /* Test if 'index' is a valid position on the hand array */
    int hand_size;

    for (hand_size = 0; hand[hand_size] != NULL; hand_size++); //Note the semicolon. This just increment hand_size.

    if (index > hand_size)
        return;

    int arr_pos;

    for(arr_pos = 0; arr[arr_pos] != NULL; arr_pos++);

    arr[arr_pos + 1] = NULL;
    arr[arr_pos] = hand[index];

    while (hand[index] != NULL ){
        hand[index] = hand[index + 1];
        index++;
    } 

}

Tile** new_set(void){

    Tile** set = malloc(sizeof(Tile) * NUMOFTILES);
    set[0] = NULL;

    return set;
}

/* Returns 1 if all the sets are valid. returns zero if not */
int check_table(Tile*** table){

    /* Iterate through the sets */
    for (int i = 0; table[i] != NULL; i++){

        if(!check_run(table[i]) && !check_group(table[i])){
            return 0;
        }

    }

    return 1;
}

Tile** make_pool(int mode){

    Tile** pool = malloc((NUMOFTILES + 2) * sizeof(Tile*));

    if (pool == NULL)
        return NULL;
    
    if (mode == 0) {

        for (int i = 0; i < (NUMOFTILES - 2); i++){
            pool[i] = malloc(sizeof(Tile));
            pool[i] -> number = (i % 13) + 1;
            if (i % 4 == 0)
                pool[i] -> suit = '!';
            if (i % 4 == 1)
                pool[i] -> suit = '@';
            if (i % 4 == 2)
                pool[i] -> suit = '#';
            if (i % 4 == 3)
                pool[i] -> suit = '$';
        }

        pool[NUMOFTILES - 1] = malloc(sizeof(Tile));
        pool[NUMOFTILES - 1] -> number = 14;
        pool[NUMOFTILES - 1] -> suit = '*';

        pool[NUMOFTILES - 2] = malloc(sizeof(Tile));
        pool[NUMOFTILES - 2] -> number = 14;
        pool[NUMOFTILES - 2] -> suit = '*';

        pool[NUMOFTILES] = NULL;
    }

    return pool;


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

    Tile** player[2];
    player[0] = malloc(MAXHAND * sizeof(Tile));
    player[1] = malloc(MAXHAND * sizeof(Tile));

    player[0][0] = NULL;
    player[1][0] = NULL;

    Tile** pool = make_pool(0);

    fisher_yates(pool);

    /* The table will hold an array of sets, which are arrays of pointers to tiles */
    Tile*** table = malloc(sizeof(Tile**) * NUMOFTILES);
    table[0] = NULL;

    for (int i = 0; i < 14; i++){
        pool_get(player[0], pool);
        pool_get(player[1], pool);
    } 

    int maxrow, maxcol;

    getmaxyx(stdscr, maxrow, maxcol);
    
    table[0] = new_set();
    table[1] = NULL;
    
    int ch = 0;
    int curpos = 0;
    
    do{

        tile_sort(player[0]);

        clear();

        mvprintw(1,1, "Left / Right to move,  Enter to add tile to array. N to get a new tile");

        if (ch == '\n'){
            arr_put(player[0], curpos, table[0]);
        }
        if (ch == 'n') {
            pool_get(player[0], pool);
        }

        int handsize;
        for (handsize = 0; player[0][handsize] != NULL; handsize++);

        if (ch == KEY_RIGHT)
            curpos++;
        if (ch == KEY_LEFT)
            curpos--;
        if (curpos < 0)
            curpos = handsize - 1;
        if (curpos >= handsize)
            curpos = 0;


        for(int i = 0; player[0][i] != NULL; i++){
            if (curpos == i){ 
                attron(A_STANDOUT);
            }
            mvprintw(maxrow - 1, (i * 7) + (maxcol - handsize * 7)/2  , "%X %c", player[0][i] -> number, player[0][i] -> suit);
            attroff(A_STANDOUT);
        }

        int tablesize;
        for (tablesize = 0; table[0][tablesize] != NULL; tablesize++);

        for (int i = 0; table[0][i] != NULL; i++){
            mvprintw(maxrow/2, (i * 7) + (maxcol - tablesize  * 7)/2  , "%X %c", table[0][i] -> number, table[0][i] -> suit);
        }
        mvprintw(maxrow/2 + 2,(maxcol - 20)/2, "Run? %s;  Group? %s;", check_run(table[0]) ? "YES" : "NO" , check_group(table[0]) ? "YES" : "NO");

        refresh();
    } while ((ch = getch()) != 'q');

    endwin();
}

int check_run (Tile** run){

    int pos = 0;
    for (pos = 1; run[pos] != NULL; pos++){
        if (run[pos] -> number == 14)
            continue;
        if (run[pos - 1] -> number == 14){

            if (pos == 1) {   // Joker is first element of the array.
                continue;
            }

            if (run[pos - 2] -> number == 14){ //There is another joker behind the first joker

                if (pos == 2){       // The two jokers are the first two elements of the array
                    continue;
                }

                else if (run[pos] -> number - 3 == run[pos - 3] -> number &&  run[pos] -> suit == run[pos - 3] -> suit){
                    continue;
                }

            }

            else if (run[pos] -> number - 2 == run[pos - 2] -> number && run[pos] -> suit == run[pos - 2] -> suit){ 
                continue;
            }

            else return 0;
        }
        else if (  (run[pos - 1] -> number) != (run[pos] -> number - 1)     // Numbers in a run must be consecutive.
                   ||    (run[pos] -> suit) != (run[pos - 1] -> suit)   ){  // Numbers in a run must have the same colour. 

            return 0;
        }

    }
    if (pos < 3){
        return 0;
    }

    return 1;
    
}

int check_group (Tile** group){

    int pos = 0;
    int group_num = -1;
    
    for (pos = 0; group[pos] != NULL; pos++); 
    if (pos < 3 || pos > 4){
        return 0;
    }

    for (pos = 0; group[pos] -> number == 14; pos++); 
        
    group_num = group[pos] -> number;

    for (pos = 0; group[pos] != NULL; pos++){
        if (group[pos] -> number == 14){
            continue;
        }
        if (group[pos] -> number != group_num){
            return 0;
        }
        for (int j = 0; j < pos; j++){
            if (group[pos] -> suit == group[j] -> suit){
                return 0;
            }
        }
    }

    return 1;

}

