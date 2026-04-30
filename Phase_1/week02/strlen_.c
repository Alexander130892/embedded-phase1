/*
 * File:    strlen_.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a custom version of the standard C library
 *   strlen() function that calculates and returns the length of a
 *   null-terminated string by iterating through each character until
 *   it reaches the null terminator. The implementation demonstrates
 *   the basic string length calculation algorithm with a test case
 *   using the alphabet string.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

//Consts

//Typedefs

//Prototype Functions
size_t strlen_(const char* test_string); //size_t used in std def (unsigned type) int technically correct
//size_t is defined as "whatever unsigned integer type is large enough to represent the largest possible object on this platform." On a 64-bit machine that's typically 8 bytes, meaning it can represent strings up to 18 exabytes.
//MAIN

int main(){
    const char * test_string = "abcdefghijklmnopqrstuvwxyz";
    printf("The test string '%s' contains %zu characters\n", test_string, strlen_(test_string));
}

size_t strlen_(const char* test_string){
    int count=0;
    while((*test_string++)!='\0'){
        count++;
    }
    return count;
}
