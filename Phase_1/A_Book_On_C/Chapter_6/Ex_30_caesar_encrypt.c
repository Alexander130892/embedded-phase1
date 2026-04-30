/*
 * File:    Ex_30_caesar_encrypt.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements Caesar cipher encryption and decryption with
 *   a shift of 3, demonstrating both encoding and decoding of a test
 *   string while preserving non-alphabetic characters. The program
 *   uses a reusable caesar() function that handles both positive and
 *   negative shifts with proper wraparound for the alphabet.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <ctype.h>

//Consts
#define CYPHER (3)

//Typedefs
typedef const char cchar;

//Prototype Functions

void caesar(char *dst, const char *src, int len, int shift);
//Consts
#define CYPHER (3)
//MAIN
int main(){
    
    char s[]="abc is a test string xyz";
    int len=sizeof(s);
    char *enc=(char *) malloc(len*sizeof(char));
    // for(int i=0; i < len; i++ )
    // {
    //     if((tolower(s[i])>='a') && (tolower(s[i])<='z'))
    //     {
    //         if((tolower(s[i])+CYPHER) <= 'z')
    //         {
    //             enc[i]=s[i]+CYPHER;
    //         }else
    //         {
    //             enc[i]=s[i]-26+CYPHER;
    //         }
    //     }
    //     else
    //     {
    //         enc[i]=s[i];
    //     }
    // }
    char *dec=(char *) malloc(len*sizeof(char));
    // for(int i=0; i < len; i++ )
    // {
    //     if((tolower(enc[i])>='a') && (tolower(enc[i])<='z'))
    //     {
    //         if((tolower(enc[i])-CYPHER) >= 'a')
    //         {
    //             dec[i]=enc[i]-CYPHER;
    //         }else
    //         {
    //             dec[i]=enc[i]+26-CYPHER;
    //         }
    //     }
    //     else
    //     {
    //         dec[i]=enc[i];
    //     }
    // }
    caesar(enc, s,   len, +CYPHER);   /* encode */
    caesar(dec, enc, len, -CYPHER);   /* decode */
    printf("%s\n%s\n%s\n",s, enc, dec);
    free(enc);
    free(dec);
    return EXIT_SUCCESS;
}


void caesar(char *dst, const char *src, int len, int shift){
    shift = ((shift % 26) + 26) % 26;   /* normalise: handles negative shifts */
    for(int i = 0; i < len; i++){
        if(isalpha((unsigned char)src[i])){
            char base = isupper((unsigned char)src[i]) ? 'A' : 'a';
            dst[i] = base + (src[i] - base + shift) % 26;
        } else {
            dst[i] = src[i];
        }
    }
}