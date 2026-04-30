/*
 * File:    Ex_17_18_19_20_polishNotation.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This C program implements a Polish (prefix) notation calculator
 *   that parses comma-separated mathematical expressions into a stack
 *   and evaluates them using basic arithmetic operations (+, -, *).
 *   The program uses a linked-list-based stack data structure to store
 *   and process operands and operators, demonstrating stack
 *   manipulation techniques for expression evaluation.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>

#define EMPTY   0
#define FULL    10000


struct data{
    enum {operator, value} kind;
    union {
        char    op;
        int     val;
    }u;
};

typedef struct data         data;
typedef enum{false, true}   boolean;

struct elem {
    data            d;
    struct elem * next;
};
typedef struct elem elem;
struct stack{
    int cnt;
    elem* top;
};
typedef struct stack stack;


//Prototypes
boolean empty(const stack *stk);
int     evaluate(stack * polish);
void    fill(stack* stk, const char* str);
boolean full(const stack* stk);
void    initialize(stack* stk);
data    pop(stack* stk);
void    print_data(data *dp);
void    print_stack(stack* stk);
void    push(data d, stack* stk);
data    top(stack* stk);

int main() {
    //char str[] = "7, 6, -, 3, *"; //3
    //char str[] = "9, 2, 3, *, 4, -, +"; //9 + ((2*3)-4) = 11
    // char str[] = "1, 2, +, 3, 4, +, *"; // (1+2)*(3+4) =21
    // char str[] = "7, 8, +,9,+, 4, *"; // (7+8+9)*4 = 96
    // char str[] = "6,2,-,5,15,2,*,+,*"; // (6-2)*(5+15*2) = 140
    char str[] = "6,2,5,*,-,15,2,*,+"; // 6-2*5+15*2 = 26
    stack polish;

    printf("\n%s%s\n\n", "Polish expression: ", str);
    fill(&polish, str);
    print_stack(&polish);
    printf("\n%s%d\n\n", "Polish evaluation: ", evaluate(&polish));
    return EXIT_SUCCESS;
}

int evaluate(stack* polish){
    data d, d1, d2;
    stack eval;

    initialize(&eval);
    while(!empty(polish)){
        d = pop(polish);
        switch(d.kind){
            case value:
                push(d, &eval);
                break;
            case operator:
                d2 = pop(&eval);
                d1 = pop(&eval);
                d.kind = value;
                switch(d.u.op){
                    case '+': d.u.val = d1.u.val + d2.u.val; break;
                    case '-': d.u.val = d1.u.val - d2.u.val; break;
                    case '*': d.u.val = d1.u.val * d2.u.val; break;
                }
                push(d, &eval);  // result goes back onto eval stack
                break;
        }
    }
    return pop(&eval).u.val;  // final result is the only element left
}

//Basic stack routines
void initialize(stack *stk){
    stk -> cnt = 0;
    stk -> top = NULL;
}
void push(data d, stack *stk){
    elem *p;
    p = malloc(sizeof(elem));
    p->d = d;
    p->next = stk -> top;
    stk -> top = p;
    stk -> cnt++;
}
void fill(stack* stk, const char* str){
    const char *p = str;
    char c1, c2;
    boolean b1, b2;
    data d;
    stack tmp;
    initialize(stk);
    initialize(&tmp);

    //First process the string and push data on tmp
    while(*p !='\0'){
        while(isspace(*p)|| *p==',') ++p;
        b1 = (boolean) ((c1 = *p) == '+' || c1 == '-' || c1=='*');
        b2 = (boolean) ((c2 = *(p+1)) == ',' || c2 == '\0');
        if(b1 && b2){
            d.kind = operator;
            d.u.op = c1;
        }else{
            d.kind = value;
            assert(sscanf(p, "%d", &d.u.val)==1);
        }
        if(!full(&tmp)){
            push(d, &tmp);
        }while(*p != ',' && *p != '\0')++p;
    }
    //now pop data from tpm and push on stk
    while(!empty(&tmp)){
        d = pop(&tmp);
        if(!full(stk))
            push(d, stk);
    }
}
void print_data(data* dp){
    switch (dp->kind)
    {
    case  operator:
        printf("%s%3c\n", "kind: operator   op:", dp->u.op);
        break;
    case  value:
        printf("%s%3d\n", "kind: value   val:", dp->u.val);
        break;
    }
}
void print_stack(stack* stk){
    data d;
    printf("stack count:%3d%s",stk->cnt, (stk->cnt==0)?"\n":"  ");
    if(!empty(stk)){
        d = pop(stk);
        print_data(&d);
        print_stack(stk);
        push(d, stk);
    }
}
boolean full(const stack *stk){
    return (boolean) (stk ->cnt == FULL);
}
boolean empty(const stack *stk){
    return (boolean) (stk ->cnt == EMPTY);
}
data pop(stack *stk){
    if(empty(stk)){         //guard
        data empty = {0};
        return empty;
    }
    data d;
    elem *p;
    d        = stk->top->d;
    p        = stk->top;
    stk->top = stk->top->next;
    stk->cnt--;
    free(p);
    return d;
}
