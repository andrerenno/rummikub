
/* TODO C# D# ** is accepted as a valid run, why? */

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>

#define MAXHAND 106
#define NUMOFTILES 106


typedef struct Tile {
    char number;
    char suit;
} Tile;

/* ==================== Function declarations ==================== */

void tile_swap(Tile**, Tile**);

/* Sorts tiles by number */
void tile_sort(Tile**);

/* Performs the fisher yates shuffle, it is used to shuffle the pool after it is initalized */
void fisher_yates(Tile**);

/* Gets a tile from the pool and puts it on the player's hand */
void pool_get(Tile**, Tile**);

/* Returns 1 if all the sets in the array are valid. returns zero if not */
int check_table(Tile***);

/* Checks a tile array to see if it is a valid run, return 1 if it is and 0 if it isn't */
int check_run (Tile**);

/* Checks a tile array to see if it is a valid group, return 1 if it is and 0 if it isn't */
int check_group (Tile**);

/* Takes a tile from the player hand and puts it on the array */
void arr_put(Tile**, unsigned int, Tile**);

/* Returns a new, dynamically allocated set */
Tile** new_set(void);

/* Creates a new pool and adds all the tiles to it, then returns it*/
Tile** make_pool(int);

void move_and_edit(int, Tile**, int*, int*);

int first_meld(Tile**, Tile**);

int sum_tiles(Tile**);

int empty_hand (Tile***);

int reg_meld(Tile**, Tile***);

/* ==================== Main ==================== */

int main(void){

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

    int num_of_players = 2; //TODO make a menu to decide the number of players

    Tile** player[num_of_players];
    int has_first_melded[num_of_players];
    int scores[num_of_players];

    for (int i = 0; i < num_of_players; i++){
        scores[i] = 0;
    }
    

    while (true) {

        for (int i = 0; i < num_of_players; i++){
            player[i] = malloc(MAXHAND * sizeof(Tile));
            player[i][0] = NULL;
            has_first_melded[i] = 0;
        }
        player[num_of_players] = NULL;

        Tile** pool = make_pool(0);
        fisher_yates(pool);


        /* The table will hold an array of sets, which are arrays of pointers to tiles */
        Tile*** table = malloc(sizeof(Tile**) * NUMOFTILES);
        table[0] = NULL;

        for (int i = 0; i < 14; i++){
            for (int j = 0; j < num_of_players; j++){
                pool_get(player[j], pool);
            }
        } 

        int maxrow, maxcol;

        getmaxyx(stdscr, maxrow, maxcol);

        int turn = 0; 
        while (!empty_hand(player)) {
            getmaxyx(stdscr, maxrow, maxcol);
            int current_player = turn % num_of_players;

            clear();

            mvprintw(maxrow / 2 - 15, (maxcol - 8)/2, "Player %d", current_player + 1);
            mvprintw(maxrow / 2, (maxcol - 28)/2, "Press any button to continue");
            getch();


            if (!(has_first_melded[current_player])){

                int set_index;
                for (set_index = 0; table[set_index] != NULL; set_index++);
                table[set_index] = new_set();
                table[set_index + 1] = NULL;

                int result =  first_meld(player[current_player], table[set_index]);
                if (result == 0){
                    has_first_melded[current_player] = 1;
                }
                else if (result == 1) {
                    pool_get(player[current_player], pool);
                    free(table[set_index]);
                    table[set_index] = NULL;
                }
                else{ 
                    clear();
                    if (result == 2)
                        mvprintw(maxrow / 2 - 15, (maxcol - 31)/2, "Invalid set! Three cards drawn.");
                    if (result == 3)
                        mvprintw(maxrow / 2 - 15, (maxcol - 68)/2, "Invalid first meld! (Cards have to add up to 30.) Three cards drawn.");

                    pool_get(player[current_player], pool);
                    pool_get(player[current_player], pool);
                    pool_get(player[current_player], pool);

                    free(table[set_index]);
                    table[set_index] = NULL;

                    mvprintw(maxrow / 2, (maxcol - 28)/2, "Press any button to continue");
                    getch();

                }
            }
            else {
                int result = reg_meld(player[current_player], table);
                if (result == 1) {
                    pool_get(player[current_player], pool);
                }
                else if (result == 2){ 
                    clear();
                    mvprintw(maxrow / 2 - 15, (maxcol - 31)/2, "Invalid set! Three cards drawn.");

                    pool_get(player[current_player], pool);
                    pool_get(player[current_player], pool);
                    pool_get(player[current_player], pool);

                    mvprintw(maxrow / 2, (maxcol - 28)/2, "Press any button to continue");
                    getch();
                }

            }

            turn++;
        }

        for (int i = 0; i < num_of_players; i++){
            if (sum_tiles(player[i]) == 0){
                for (int j = 0; j < num_of_players; j++)
                    scores[i] += sum_tiles(player[j]);
            }
            else {
                scores[i] -= sum_tiles(player[i]);
            }
        }
        clear();
        mvprintw(maxrow/2 - 6, (maxcol - 6)/2, "Scores");
        for (int i = 0; i < num_of_players; i++){
            mvprintw(maxrow / 2 - 1, (maxcol - 2) - 10*i, "Player %d", i+1);
            mvprintw(maxrow / 2, (maxcol - 2) - 10*i, "%d", scores[i]); 
        }
        mvprintw(maxrow / 2 + 5, (maxcol - 51)/2, "Press <ENTER> to play again, anything else to quit.");

        int ch = getch();

        if (ch == '\n')
            continue;

        else break;
        
    }
    endwin();
}

/* ==================== Functions ==================== */

void tile_swap(Tile **a1, Tile **a2){
    Tile* temp = *a1;
    *a1 = *a2;
    *a2 = temp;
}

void tile_sort(Tile* arr[]){

    int size;
    for (size = 0; arr[size] != NULL; size++); 
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size - 1; j++)
            if (arr[j] -> number > arr[j + 1] -> number){ 
                tile_swap(&arr[j], &arr[j + 1]);
            }
}

void fisher_yates(Tile* arr[]){

    int randint;
    int size = 0;
    srand(time(NULL));
    for (size = 0; arr[size] != NULL; size++);
    for (int i = (size - 1); i > 0; i--){
        randint = rand() % (i + 1);	
        tile_swap(&arr[i], &arr[randint]);
    } 
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
    unsigned int hand_size;

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

int check_table(Tile*** table){

    /* Iterate through the sets */
    for (int i = 0; table[i] != NULL; i++){

        if(!check_run(table[i]) && !check_group(table[i])){
            return 0;
        }

    }

    return 1;
}

int check_run (Tile** run){
    int pos = 0;
    for (pos = 1; run[pos] != NULL; pos++){
        if (abs(run[pos] -> number) == 14)
            continue;
        if (abs(run[pos - 1] -> number) == 14){

            if (pos == 1) {   // Joker is first element of the array.
                continue;
            }

            if (abs(run[pos - 2] -> number == 14)){ //There is another joker behind the first joker

                if (pos == 2){       // The two jokers are the first two elements of the array
                    continue;
                }

                else if (abs(run[pos] -> number) - 3 == abs(run[pos - 3] -> number) && run[pos] -> suit == run[pos - 3] -> suit){
                    continue;
                }

                else return 0;

            }

            else if (abs(run[pos] -> number) - 2 == abs(run[pos - 2] -> number) && run[pos] -> suit == run[pos - 2] -> suit){ 
                continue;
            }

            else return 0;
        }
        else if (abs(run[pos - 1] -> number) != abs(run[pos] -> number) - 1     // Numbers in a run must be consecutive.
                ||          (run[pos] -> suit) != (run[pos - 1] -> suit)   ){  // Numbers in a run must have the same colour. 

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

    for (pos = 0; abs(group[pos] -> number) == 14; pos++); 

    group_num = abs(group[pos] -> number);

    for (pos = 0; group[pos] != NULL; pos++){
        if (abs(group[pos] -> number) == 14){
            continue;
        }
        if (abs(group[pos] -> number) != group_num){
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

void move_and_edit(int ch, Tile* arr[], int* curpos, int* edit){
    int handsize;
    for (handsize = 0; arr[handsize] != NULL; handsize++);

    if (ch == KEY_RIGHT){ 
        if (*edit == 1){
            tile_swap(&arr[*curpos], &arr[(*curpos + 1) % handsize]);   
        }
        (*curpos)++;
    }
    if (ch == KEY_LEFT){
        if (*edit == 1){
            tile_swap(&arr[*curpos], &arr[(*curpos == 0 ? (handsize - 1) : *curpos - 1)]);   
        }
        (*curpos)--;
    }

    if (ch == 'e' && *edit == 0){
        *edit = 1;
        ch = 0;
    }
    if (ch == 'e' && *edit == 1){
        *edit = 0;
        ch = 0;
    }

    if (*curpos < 0)
        *curpos = handsize - 1;
    if (*curpos >= handsize)
        *curpos = 0;

}

int first_meld(Tile* hand[], Tile* set[]){

    int maxrow, maxcol;
    int ch = 0;
    int curpos = 0;
    int edit = 0;
    int place = 0;

    do{

        getmaxyx(stdscr, maxrow, maxcol);

        clear();


        if (ch == 't'){
            place = 1;
            edit = 0;
        }

        if (ch == 'h'){
            place = 0;
            edit = 0;
        }

        if (set[0] == NULL)
            place = 0;
        if (hand[0] == NULL)
            place = 1;

        if (place == 0 && ch == ' '){
            arr_put(hand, curpos, set);
            if (hand[0] == NULL){ 
                place = 1;
            }

        }
        else if (place == 1 && ch == ' '){
            arr_put(set, curpos, hand);
            if (set[0] == NULL){ 
                place = 0;
            }
        }

        if (place == 0){
            move_and_edit(ch, hand, &curpos, &edit);
        }
        if (place == 1){
            move_and_edit(ch, set, &curpos, &edit);
        }

        int handsize;
        for (handsize = 0; hand[handsize] != NULL; handsize++);


        for(int i = 0; hand[i] != NULL; i++){
            move(maxrow - 1, (i * 7) + (maxcol - handsize * 7)/2);
            if (curpos == i && place == 0){ 
                if (edit == 1)
                    move(maxrow - 2, (i * 7) + (maxcol - handsize * 7)/2);
                attron(A_STANDOUT);
            }
            printw("%X %c", hand[i] -> number, hand[i] -> suit);
            attroff(A_STANDOUT);
        }

        int set_size;
        for (set_size = 0; set[set_size] != NULL; set_size++);

        for (int i = 0; set[i] != NULL; i++){
            move(maxrow / 2, (i * 7) + (maxcol - set_size * 7)/2);
            if (curpos == i && place == 1){ 
                if (edit == 1)
                    move(maxrow / 2 - 1 , (i * 7) + (maxcol - set_size * 7)/2);
                attron(A_STANDOUT);
            }
            printw("%X %c", set[i] -> number, set[i] -> suit);
            attroff(A_STANDOUT);
        }

        refresh();

    } while ((ch = getch()) != '\n');

    if(!check_run(set) && !check_group(set)){
        if (set[0] == NULL)
            return 1;

        while (set[0] != NULL){
            arr_put(set, 0, hand);
        }
        return 2;
    }
    if (sum_tiles(set) < 30)
        return 3;


    return 0;

}

int sum_tiles(Tile* set[]){
    int sum = 0; 
    if (check_group(set)){
        int pos;
        for (pos = 0; set[pos] -> number == 14; pos++); 
        int group_num = set[pos] -> number;

        for (pos = 0; set[pos] != NULL; pos++){
            sum += group_num;
        }
        return sum;
    }

    if (check_run(set)){
        int pos;
        for (pos = 0; set[pos] -> number == 14; pos++); 
        int run_num = set[pos] -> number - pos;

        for (pos = 0; set[pos] != NULL; pos++){
            sum += run_num + pos;
        }
        return sum;
    }

    return 0;

}

int empty_hand (Tile*** player){

    for (int i = 0; player[i] != NULL; i++){
        if (player[i][0] == NULL)
            return (i + 1);
    }

    return 0;

}

int reg_meld(Tile* hand[], Tile** table[]){
    int maxrow, maxcol;
    int ch = 0;
    int curpos = 0;
    int setpos = 0;
    int edit = 0;
    int place = 0;


    Tile*** table_cpy = malloc(sizeof(Tile**) * NUMOFTILES);

    int cpy_set;
    for (cpy_set = 0; table[cpy_set] != NULL; cpy_set++){
        table_cpy[cpy_set] = new_set();
        int j;
        for (j = 0; table[cpy_set][j] != NULL; j++){
            table_cpy[cpy_set][j] = table[cpy_set][j];
        }
        table_cpy[cpy_set][j] = NULL;
    }
    table_cpy[cpy_set] = NULL;

    do{

        getmaxyx(stdscr, maxrow, maxcol);
        clear();

        int table_size;
        for (table_size = 0; table[table_size] != NULL; table_size++);

        if (ch == 't'){
            place = 1;
            edit = 0;
        }

        if (ch == 'h'){
            place = 0;
            edit = 0;
        }
        if (ch == 'n'){
            int i;
            for (i = 0; table[i] != NULL; i++){
                if (table[i][0] == NULL){
                    break;
                }
            }
            if (table[i] != NULL){
                mvprintw(2, (maxcol - 69)/2, "It looks like you already have an empty set. Try filling it up first.");
            }
            else{ 
                table[table_size] = new_set();
                table[++table_size] = NULL;
            }

        }
        if (place == 0 && ch == ' '){
            hand[curpos] -> number *= -1;
            arr_put(hand, curpos, table[setpos]);
            if (hand[0] == NULL){ 
                place = 1;
            }

        }
        else if (place == 1 && ch == ' '){
            if (table[setpos][curpos] -> number < 0){ 
                table[setpos][curpos] -> number *= -1;
                arr_put(table[setpos], curpos, hand);
            }
            else 
                mvprintw(2, (maxcol - 55)/2, "You can't take back a tile you didn't place this round.");

            if (table[setpos][0] == NULL){ 
                place = 0;
            }
        }


        if (ch == KEY_DOWN && edit == 1){
            arr_put(table[setpos], curpos, table[(setpos + 1) % table_size]);
            setpos = (setpos + 1) % table_size;
            for(curpos = 0; table[setpos][curpos] != NULL; curpos++);
            curpos--;

        }
        else if (ch == KEY_DOWN)
            setpos = (setpos + 1) % table_size;

        if (ch == KEY_UP && edit == 1){
            arr_put(table[setpos], curpos, table[setpos - 1 < 0 ? table_size - 1 : setpos - 1 ]);
            setpos = setpos - 1 < 0 ? table_size - 1 : setpos - 1;
            for(curpos = 0; table[setpos][curpos] != NULL; curpos++);
            curpos--;
        }
        else if (ch == KEY_UP)
            setpos = setpos - 1 < 0 ? table_size - 1 : setpos - 1;




        if (place == 0){
            move_and_edit(ch, hand, &curpos, &edit);
        }

        if (place == 1){
            move_and_edit(ch, table[setpos], &curpos, &edit);
        }

        int handsize;
        for (handsize = 0; hand[handsize] != NULL; handsize++);


        for(int i = 0; hand[i] != NULL; i++){
            move(maxrow - 1, (i * 7) + (maxcol - handsize * 7)/2);
            if (curpos == i && place == 0){ 
                if (edit == 1)
                    move(maxrow - 2, (i * 7) + (maxcol - handsize * 7)/2);
                attron(A_STANDOUT);
            }
            printw("%X %c", abs(hand[i] -> number), hand[i] -> suit);
            attroff(A_STANDOUT);
        }

        for (int j = 0; table[j] != NULL; j++){
            Tile** set = table[j];

            int set_size;
            for (set_size = 0; set[set_size] != NULL; set_size++);

            mvprintw(maxrow / 2 - 20 + 2*j,  (maxcol - set_size * 7 - 30)/2, "Set %d", j);

            for (int i = 0; set[i] != NULL; i++){
                move(maxrow / 2 - 20 + 2*j, (i * 7) + (maxcol - set_size * 7)/2);
                if (curpos == i && place == 1 && setpos == j){ 
                    if (edit == 1)
                        move(maxrow / 2 - 20 + 2 * j - 1 , (i * 7) + (maxcol - set_size * 7)/2);
                    attron(A_STANDOUT);
                }
                printw("%X %c", abs(set[i] -> number), set[i] -> suit);
                attroff(A_STANDOUT);
            }
        }

        refresh();

    } while ((ch = getch()) != '\n');

    if (!check_table(table)){ 
        for (int i = 0; table[i] != NULL; i++){
            for (int j = 0; table[i][j] != NULL;){
                if (table[i][j] -> number < 0){
                    table[i][j] -> number *= -1;
                    arr_put(table[i], j, hand);
                }
                else j++;

            }
            free(table[i]);
            table[i] = table_cpy[i];
        } 
        return 2;
    }
    else {
        int changes = 0;
        for (int i = 0; table[i] != NULL; i++){
            for (int j = 0; table[i][j] != NULL;j++){
                if (table[i][j] -> number < 0){
                    table[i][j] -> number *= -1;
                    changes++;
                }
            }
            free(table_cpy[i]);
        } 

        if (changes == 0)
            return 1;

        return 0;
    }




}

