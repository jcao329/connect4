#include "computer_agent.h"

/*
 * In this file, implement your AI agent.
 *
 * Use the minimax algorithm to determine the next move.
 *
 * Look at player_agent.h and player_agent.c for an example of how to
 * implement each function.
 */

#include "random_agent.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

/*
 * In this file, put your implementation of a 'random agent', i.e. a player
 * who will make a VALID but random move.
 */

typedef struct
{
   char * name;
} computer_agent_data_t;

static
const char * computer_agent_describe (agent_t * gthis)
{
   return "Computer ";
}


int minimax(board_t * b, int depth, player_t * player, int max, 
            player_t color){

    //base cases
    if(depth == 0 || board_has_winner(b, player)){
        if(board_has_winner(b, player) == 1 && *player == color){
            return 1000; //board has winner and it is color
        }
        else if(board_has_winner(b, player) == 1 && *player != color){
            return -1000; //board has winner and it is not color 
        }
        else{
            return 0; // board has tie
        }
    }
    
    if(max){
        int maxeval = -1000;
        for(unsigned int i=0; i< board_get_width(b); i++){
            if(board_can_play_move(b, color, i)){
                board_play(b, i, color);
                int result = minimax(b, depth -1, player, 0, color);
                board_unplay(b, i);
                maxeval = (maxeval > result) ? maxeval:result;
            }
        }
        return maxeval;
    }
    else{
        int mineval = 1000;
        for(unsigned int i=0; i< board_get_width(b); i++){
            if(board_can_play_move(b, color, i)){
                board_play(b, i, color);
                int result = minimax(b, depth -1, player, 1, color);
                board_unplay(b, i);
                mineval = (mineval < result) ? result:mineval;
            }
        }
        return mineval;
    }
}

static
int computer_agent_play (agent_t * gthis, const board_t * b,
        player_t color, unsigned int * move)
{
    int depth = board_get_width(b)*board_get_height(b);
    board_t * temp;
    board_duplicate(&temp, b);
    int bestScore = -1000;
    int bestMove = 0;

    for(unsigned int i=0; i<board_get_width(b); i++){
        if(board_can_play_move(temp, color, i)){
        board_play(temp, i, color);
        player_t next = (color == PLAYER_BLUE) ? PLAYER_YELLOW:PLAYER_BLUE;
        int score = minimax(temp, depth-1, &next, 0, next);
        board_unplay(temp, i);
            if(score>bestScore){
                bestScore = score;
                bestMove = i;
            }

        }
    }
    *move = bestMove;
    return 0;
}

static
bool computer_agent_destroy (agent_t * this)
{
   computer_agent_data_t * data = (computer_agent_data_t *) this->data;
   free (this->data);
   return true;
}

agent_t * computer_agent_create ()
{
    // Allocate vtable structure
    agent_t * n = malloc (sizeof(agent_t));

    n->destroy = computer_agent_destroy;
    n->play = computer_agent_play;
    n->describe = computer_agent_describe;

    computer_agent_data_t * data = malloc (sizeof(computer_agent_data_t));
    n->data = data;

    return n;
}

