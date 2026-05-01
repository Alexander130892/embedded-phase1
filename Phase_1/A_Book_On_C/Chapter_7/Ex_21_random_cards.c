/*
 * File:    Ex_21_random_cards.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This program simulates drawing 5 random playing cards from a
 *   standard deck, ensuring no duplicates are selected, and then
 *   displays the hand with each card's rank and suit. It uses
 *   rejection sampling to avoid drawing the same card twice by
 *   regenerating random cards until a unique one is found.
 */
//write pack into one line
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef enum{HEARTS, SPADES, DIAMONDS, CLUBS, COUNT_SUITS} suit;
const char* g_suit [4] = {"Hearts","Spades","Diamonds","Clubs"};
typedef enum{ILLEGAL, ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, COUNT_PIPS} pips; // ILLEGAL = 0 so valid pip indices start at 1
const char* g_pips [14] = {"ILLEGAL","Ace","Deuce","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen", "King"};

void pick_card(suit *p_suits, pips *p_pips, int * p_nbr_cards);
void print_hand(suit *p_suits, pips *p_pips, int nbr_cards);

int main(void){
    srand(time(NULL));
    suit suits[5]={0};
    pips pips[5]={0};
    int nbr_cards_in_hand=0;
    for (int card = 1 ; card <=5 ; card++){
        pick_card(suits, pips,&nbr_cards_in_hand);
    }
    print_hand(suits, pips, nbr_cards_in_hand);
    return EXIT_SUCCESS;
}

void pick_card(suit *p_suits, pips *p_pips, int * p_nbr_cards){
    int valid_pick=0;
    suit new_suit;
    pips new_pips;
    while(!valid_pick){
        new_suit = rand() % 4;
        new_pips = rand() % 13+1;
        valid_pick = 1;
        // check if we already have that card in hand, if so, illegal draw --> retry
        for (int i = 0 ; i < *p_nbr_cards ; i++){
            if(new_suit == p_suits[i] && new_pips == p_pips[i]){
                valid_pick=0;
                break;
            }
        }
    }
    p_pips[*p_nbr_cards]=new_pips;
    p_suits[*p_nbr_cards]=new_suit;
    (*p_nbr_cards)++;
    return;
}
void print_hand(suit *p_suits, pips *p_pips, int nbr_cards){
    for(int index = 0; index < nbr_cards ; index++){
        printf("%d: %s of %s ",(index+1), g_pips[p_pips[index]], g_suit[p_suits[index]]);
        putchar('\n');
    }
    putchar('\n');
    return;
}