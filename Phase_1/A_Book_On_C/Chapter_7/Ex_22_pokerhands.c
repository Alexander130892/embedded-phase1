/*
 * File:    Ex_22_pokerhands.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This C program simulates dealing 100,000 random 5-card poker hands
 *   and calculates the statistical probabilities of getting a flush,
 *   straight, and full house by counting occurrences and displaying
 *   the percentages. It also prints the first occurrence of each hand
 *   type during the simulation.
 */
//write pack into one line
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum{HEARTS, SPADES, DIAMONDS, CLUBS, COUNT_SUITS} suit;
const char* g_suit [4] = {"Hearts","Spades","Diamonds","Clubs"};
typedef enum{ILLEGAL, ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, COUNT_PIPS} pips; // ILLEGAL = 0 so valid pip indices start at 1
const char* g_pips [14] = {"ILLEGAL","Ace","Deuce","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen", "King"};

#define COUNT 100000.0
int flush_count = 0, straight_count=0, full_house_count=0;

void pick_card(suit *p_suits, pips *p_pips, int * p_nbr_cards);
void print_hand(suit *p_suits, pips *p_pips, int nbr_cards);
int check_flush(suit *p_suits, pips *p_pips, int* already_printed, int draw);
void sort_hand(suit *p_suits, pips *p_pips, int nbr_cards);
void check_straight(suit *p_suits,pips *p_pips, int* already_printed, int draw);
bool is_straight(pips *p_pips, int nbr_cards);
void check_full_house(suit *p_suits, pips *p_pips, int* already_printed, int draw);

int main(void)
{
    srand(time(NULL));

    int flush_printed = 0, straight_printed = 0, full_house_printed = 0, draw = 0;

    suit suits[5] = {0};
    pips pips[5] = {0};

    while (draw < COUNT)
    {
        draw++;
        int nbr_cards_in_hand = 0;
        for (int card = 1; card <= 5; card++)
        {
            pick_card(suits, pips, &nbr_cards_in_hand);
        }

        check_flush(suits, pips, &flush_printed, draw);
        check_straight(suits, pips, &straight_printed, draw);
        check_full_house(suits, pips, &full_house_printed, draw);
    }
    printf("Straight %%: %.2f%%\n", straight_count/COUNT*100);
    printf("Flush %%: %.2f%%\n", flush_count/COUNT*100);
    printf("Full House %%: %.2f%%\n", full_house_count/COUNT*100);
    return EXIT_SUCCESS;
}

void pick_card(suit *p_suits, pips *p_pips, int *p_nbr_cards)
{
    int valid_pick = 0;
    suit new_suit;
    pips new_pips;
    int count=0;
    while (!valid_pick)
    {
        new_suit = rand() % 4;
        new_pips = rand() % 13 + 1;
        valid_pick = 1;

        // Check if we already have that card in hand
        for (int i = 0; i < *p_nbr_cards; i++)
        {
            if (new_suit == p_suits[i] && new_pips == p_pips[i])
            {
                valid_pick = 0;
                break;
            }
        }
    }

    // Add the new card to the hand
    p_pips[*p_nbr_cards] = new_pips;
    p_suits[*p_nbr_cards] = new_suit;
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
int check_flush(suit *p_suits, pips *p_pips, int* already_printed, int draw){
    if ((p_suits[0] == p_suits[1]) && (p_suits[0] == p_suits[2]) && (p_suits[0] == p_suits[3]) && (p_suits[0] == p_suits[4])) 
    {
        if ((*already_printed)!=1){
            printf("1st Flush at draw %d:\n", draw);
            print_hand(p_suits, p_pips, 5);
            *already_printed =1;
            flush_count++;
        }else
            flush_count++;
    }
    return *already_printed;
}
void check_straight(suit *p_suits,pips *p_pips, int* already_printed, int draw)
{
    sort_hand(p_suits,p_pips, 5);
    if (is_straight(p_pips, 5))
    {
        straight_count++;
        if(already_printed != 0){
            printf("1st Straight at draw: %d\n", draw);
            *already_printed =1;
            print_hand(p_suits, p_pips, 5);
        }
    }
    return;
}
void sort_hand(suit *p_suits, pips *p_pips, int nbr_cards)
{
    for (int i = 0; i < nbr_cards - 1; i++)
    {
        for (int j = 0; j < nbr_cards - i - 1; j++)
        {
            if (p_pips[j] > p_pips[j + 1])
            {
                // swap pips
                pips temp_pip   = p_pips[j];
                p_pips[j]       = p_pips[j + 1];
                p_pips[j + 1]   = temp_pip;
                // swap suits in tandem
                suit temp_suit  = p_suits[j];
                p_suits[j]      = p_suits[j + 1];
                p_suits[j + 1]  = temp_suit;
            }
        }
    }
}
bool is_straight(pips *p_pips, int nbr_cards)
{
    // Check for normal straight (e.g., 5,6,7,8,9)
    bool straight = true;
    for (int i = 1; i < nbr_cards; i++)
    {
        if (p_pips[i] != p_pips[i - 1] + 1)
        {
            straight = false;
            break;
        }
    }
    if (straight)
        return true;

    // Check for Ace-low straight (Ace,2,3,4,5)
    if (p_pips[0] == ACE && p_pips[1] == TWO && p_pips[2] == THREE && p_pips[3] == FOUR && p_pips[4] == FIVE)
        return true;

    // Check for Ace-high straight (10,J,Q,K,Ace)
    if (p_pips[0] == TEN && p_pips[1] == JACK && p_pips[2] == QUEEN && p_pips[3] == KING && p_pips[4] == ACE)
        return true;

    return false;
}
void check_full_house(suit *p_suits, pips *p_pips, int* already_printed, int draw)
{
    sort_hand(p_suits,p_pips, 5);

    // Check for three of a kind and a pair
    if (
        (p_pips[0] == p_pips[1] && p_pips[1] == p_pips[2] && p_pips[3] == p_pips[4]) ||
        (p_pips[0] == p_pips[1] && p_pips[2] == p_pips[3] && p_pips[3] == p_pips[4])
    )
    {
        full_house_count++;
        if (*already_printed != 1)
        {
            printf("1st Full House at draw: %d\n", draw);
            *already_printed = 1;
            print_hand(p_suits, p_pips, 5);
        }
    }
    return;
}