/*
 * File:    memset_.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a custom version of the standard C library
 *   function `memset`, which fills a block of memory with a specified
 *   byte value. The implementation uses a simple loop to set each byte
 *   and includes comprehensive tests demonstrating typical usage
 *   patterns and edge cases like value truncation and zero-length
 *   operations.
 */


/*
 * File:    memset_.c
 * Author:  Alexander Vindelinckx
 * Date:    22-3-2026
 *
 * Description:
 *   set memory at adress s, for the following n byets, to c
 */

 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

//Consts

//Typedefs

//Prototype Functions
void *memset_(void *s, int c, size_t n);

int main(){ //main written by Claude
    // Test 1 — zero out a buffer
    char buf1[10] = "hello";
    memset_(buf1, 0, sizeof(buf1));
    printf("Test 1 (zero buffer): ");
    for(int i = 0; i < 10; i++)
        printf("%d ", buf1[i]);  // expect all 0s
    putchar('\n');

    // Test 2 — fill with a character
    char buf2[6];
    memset_(buf2, 'A', 5);
    buf2[5] = '\0';
    printf("Test 2 (fill 'A'):    %s\n", buf2);  // expect AAAAA

    // Test 3 — fill only part of a buffer
    char buf3[10] = "hello";
    memset_(buf3, '*', 3);
    buf3[9] = '\0';
    printf("Test 3 (partial):     %s\n", buf3);  // expect ***lo + ?

    // Test 4 — n=0 edge case
    char buf4[5] = "hello";
    memset_(buf4, 'X', 0);
    printf("Test 4 (n=0):         %c\n", buf4[0]);  // expect 'h', unchanged

    // Test 5 — value truncation, c > 255
    char buf5[4];
    memset_(buf5, 0x141, 3);  // 0x141 truncates to 0x41 = 'A'
    buf5[3] = '\0';
    printf("Test 5 (truncation):  %s\n", buf5);  // expect AAA

    return EXIT_SUCCESS;
}

void * memset_(void *s, int c, size_t n){
    unsigned char* mem=(unsigned char*) s;
    for(int index = 0; index < n ; index ++){
        *(mem+index) = (unsigned char) c;
    }
    return s;
}
