#include "board.h"
#include <stdlib.h>
/**
 * In this file, implement all of the functions for board.h
 *
 * Once done, make sure you pass all the tests in board_test.c
 * (make test_board)
 *
 */

int board_vert_winner(const board_t *b, player_t *player);
int board_horiz_winner(const board_t *b, player_t *player);
int board_diag_winner(const board_t *b, player_t *player);


typedef struct board_t{
    unsigned int width;
    unsigned int height;
    unsigned int run;
    player_t * grid;
};

bool board_create (board_t ** b, unsigned int height,
        unsigned int width, unsigned int run,
        const player_t * initial){

            if (height == 0 || width == 0 || run == 0){
                printf("Improper dimensions \n");
                return false;
            }

            *b = malloc(sizeof(struct board_t));

            if (*b == NULL){
                printf("Memory allocation error \n");
                return false;
            }

            (*b)->width = width;
            (*b)->height = height;
            (*b)->run = run;
            (*b)->grid = malloc(width * height * sizeof(player_t));
            
            if((*b)->grid == NULL){
                printf("Memory allocation error for grid \n");
                return false;
            }

            if(initial == NULL){
                for (unsigned int i = 0; i < width * height; i++) {
                    (*b)->grid[i] = PLAYER_EMPTY;
                }
            }
            else {
                for (unsigned int i = 0; i < width * height; i++) {
                    (*b)->grid[i] = initial[i];        
                }
            }

            return true;
        }

bool board_destroy (board_t * b){
    if(b == NULL){
        return false;
    }

    free(b->grid);
    free(b);
    return true;
}

unsigned int board_get_height (const board_t * b){
    return b->height;
}

unsigned int board_get_width (const board_t * b){
    return b->width;
}

unsigned int board_get_run (const board_t * b){
    return b->run;
}

bool board_play (board_t * b, unsigned int column, player_t player){
    if(b == NULL){
        printf("Not a board\n");
        return false;
    }

    if (column > b->width){
        return false;
    }

    if (player == PLAYER_EMPTY){
        return false;
    }

    for(unsigned int i = 0; i<b->height; i++){
        unsigned int index = i*b->width + column;
        if (b->grid[index] == PLAYER_EMPTY){
            b->grid[index] = player;
            return true;
        }
    }

    return false;
}

bool board_can_play (const board_t * b, player_t p){
    if(b == NULL || p == PLAYER_EMPTY){
        return false;
    }
    for(unsigned int i = 0; i<b->height; ++i){
        for(unsigned int j = 0; j< b->width; ++j){
            int index = i * b->width + j;
            if(b->grid[index] == PLAYER_EMPTY){
                return true;
            }
        }
    }
    return false;
}

bool board_can_play_move (const board_t * b, player_t p, unsigned int
      column){
        if(b == NULL || column > b->width){
        return false;
        }

        for(unsigned int i = 0; i < b->height; i++){
            unsigned int index = i*b->width + column;
            if(b->grid[index] == PLAYER_EMPTY){
                return true;
            }
        }
        return false;
        
      }

bool board_unplay (board_t * b, unsigned int column){
    if(b == NULL || column >= b->width){
        return false;
        }
    for(int i = b->height - 1 ; i >= 0; i--){
            unsigned int index = i*b->width + column;
            if(b->grid[index] != PLAYER_EMPTY){
                b->grid[index] = PLAYER_EMPTY;
                return true;
            }
        }
    return false;
}

bool board_duplicate (board_t ** newboard, const board_t * old){
    if(old == NULL){
        return false;
    }
    return board_create(newboard, old->height, old->width, old->run, old->grid);
}



int board_vert_winner(const board_t *b, player_t *player){
    for(unsigned int j = 0; j < b->width; j++){
        int longest_run = 0;
        player_t p = PLAYER_EMPTY;
        for(unsigned int i = 0; i < b->height; i++){
            if(b->grid[i*b->width + j] == p && p!= PLAYER_EMPTY){
                longest_run++;
            }
            else {
                longest_run = 1;
                p = b->grid[i * b->width + j];
            }
             
            if(longest_run == b->run){
                *player = p;
                return 1;
            }
        }
    }

    return 0;
}

int board_horiz_winner(const board_t *b, player_t *player){

    for(unsigned int i = 0; i < b->height; i++){
        int longest_run = 0;
        player_t p = PLAYER_EMPTY;
        for(unsigned int j = 0; j < b->width; j++){
            if(b->grid[i*b->width + j] == p && p!= PLAYER_EMPTY){
                longest_run++;
            }
            else{
                longest_run = 1;
                p = b->grid[i*b->width + j];
            }
            if(longest_run == b->run){
                *player = p;
                return 1;
            }
        }
    }
    return 0;
}

int board_diag_winner(const board_t *b, player_t *player) {
    if (b == NULL 
        || player == NULL 
        || b->grid == NULL 
        || b->run > b->height 
        || b->run >b->width
    ) {
        return 0;
    }


    for(unsigned int i = 0; i <= b->height - b->run; i++) {
        for(unsigned int j = 0; j <= b->width - b->run; j++) {
            player_t p = b->grid[i * b->width + j];
            if (p == PLAYER_EMPTY) {
                continue;
            }

            int run = 1;
            while (run < b->run &&
                   i + run < b->height &&  
                   j + run < b->width &&   
                   b->grid[(i + run) * b->width + (j + run)] == p) {
                run++;
            }
            if (run == b->run) {
                *player = p;
                return 1;
            }
        }
    }

    // Check top-right to bottom-left diagonals
    for (unsigned int i = 0; i <= b->height - b->run; i++) {
        for (unsigned int j = b->run - 1; j < b->width; j++) {
            player_t p = b->grid[i * b->width + j];
            if (p == PLAYER_EMPTY) {
                continue;
            }

            int run = 1;
            while (run < b->run &&
                   i + run < b->height &&  
                   j >= run &&   
                   b->grid[(i + run) * b->width + (j - run)] == p) {
                run++;
            }
            if (run == b->run) {
                *player = p;
                return 1;
            }
        }
    }
    return 0;
    }



int board_has_winner(const board_t *b, player_t *player) {
    player_t p1 = PLAYER_EMPTY;
    player_t p2 = PLAYER_EMPTY;
    player_t p3 = PLAYER_EMPTY;


    if(board_vert_winner(b, &p1) == 1){
        *player = p1;
        return 1;
    }
    else if(board_horiz_winner(b, &p2) == 1){
        *player = p2;
        return 1; 
    }
    else if(board_diag_winner(b, &p3) == 1){
        *player = p3;
        return 1;
     
    }
    else if(board_can_play(b, PLAYER_BLUE)){
        return 0;
    }
    else{
        return -1;
    }
}

bool board_get(const board_t *b, unsigned int row, unsigned int column, player_t *piece) {
    if (!b || !piece || row >= b->height || column >= b->width) {
        return false;
    }
    *piece = b->grid[row * b->width + column];
    return true;
}


bool board_clear (board_t * b){
    if (!b) {
        return false;
    }

    for(unsigned int i = 0; i < b->height; i++){
        for(unsigned int j = 0; j < b->width; j++){
            b->grid[i*b->width + j] = PLAYER_EMPTY;
        }
    }
    return true;
}

bool board_print(const board_t *b, FILE *f) {
    if (!b || !f) {
        return false;
    }
    for (int row = b->height - 1; row >= 0; row--) {
        for (unsigned int col = 0; col < b->width; col++) {
            player_t piece = b->grid[row * b->width + col];
            if (piece == PLAYER_BLUE) {
                fprintf(f, "B ");
            } else if (piece == PLAYER_YELLOW) {
                fprintf(f, "Y ");
            } else {
                fprintf(f, ". ");
            }
        }
        fprintf(f, "\n");
    }
    return true;
}