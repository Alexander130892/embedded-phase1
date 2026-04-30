/*
 * File:    Ex_04_a_b_c.h
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This header file defines a macro A_B_C that expands to a complete
 *   main function which prints "A Big Cheery "hello"!" to the console.
 *   The macro uses line continuation backslashes to pack the entire
 *   function definition into what appears as a single line when the
 *   macro is invoked.
 */
//write pack into one line


#define TRUE 1
#define A_B_C int main(void)    \
                {               \
                    printf("A Big Cheery \"hello\"!\n"); \
                    return 0;   \
                }

