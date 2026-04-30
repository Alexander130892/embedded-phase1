/*
 * File:    Ex_13_roulette.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file implements a console-based roulette game where the
 *   player starts with a $5 budget and can bet on specific numbers
 *   (0-35), even, or odd outcomes, with corresponding payouts of 35x,
 *   2x, or 2x respectively. The game continues until the player runs
 *   out of money, with each spin costing $1 and using random number
 *   generation to simulate the roulette wheel.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Typedefs
const int g_start_budget = 5;
int g_plays=0;

typedef enum {
    BET_NUMBER,
    BET_EVEN,
    BET_ODD,
    BET_INVALID
} BetType;

typedef struct {
    BetType type;
    int     value;   /* only meaningful when type == BET_NUMBER */
} Bet;


//Prototype Functions
Bet place_bet(void);
static int play_roulette(void);

//MAIN
int main(void){
    int budget = g_start_budget;
    int play=-1;
    while(budget >= 1){
        printf("Game %d: Your current budget is: %d$\n", g_plays+1, budget);
        budget--;
        Bet b = place_bet();
        play = play_roulette();
        switch (b.type) {
            case BET_EVEN:   
                if(play !=0 && play % 2 == 0){
                    budget+=2;
                    printf("You won 2$ !\n");
                }else
                    printf("You lost 1$\n");
                break;
            case BET_ODD:    
                if(play !=0 && play % 2 != 0){
                    budget+=2;
                    printf("You won 2$ !\n");
                }else
                    printf("You lost 1$\n");
                break;
            case BET_NUMBER: 
                if(play !=0 && play == b.value){
                    budget+=35;
                    printf("You won 35$ !\n");
                }else
                    printf("You lost 1$\n");
                break;
            default: break;
        }
    }
    printf("Out of money :( %d$\n", budget--);
    //roulette
    //payout
    return EXIT_SUCCESS;
}

Bet place_bet(void) {
    char input[16];
    Bet bet;

    while (1) {
        printf("Choose a number 0-35 or Even(E)/Odd(O): ");
        scanf("%15s", input);

        char c = toupper(input[0]);
        if (c == 'E' && strlen(input) == 1) { bet = (Bet){BET_EVEN, 0};  break; }
        if (c == 'O' && strlen(input) == 1) { bet = (Bet){BET_ODD,  0};  break; }

        char *end;
        long val = strtol(input, &end, 10);
        if (*end == '\0' && val >= 0 && val <= 35) {
            bet = (Bet){BET_NUMBER, (int)val};
            break;
        }

        printf("Invalid. Try again.\n");
    }
    return bet;
}
int play_roulette(void){
    g_plays++;
    int play=-1;
    srand(time(NULL));
    play = rand() % 36;
    printf("The value rolled is: %d\n", play);
    return play;
}