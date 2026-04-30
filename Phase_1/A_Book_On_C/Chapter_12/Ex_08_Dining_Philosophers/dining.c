/*
 * File:    dining.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file implements a solution to the classic dining philosophers
 *   problem using 5 philosophers who must share chopsticks to eat,
 *   with each philosopher running as a separate process created via
 *   fork(). The code uses pipes to implement semaphores for
 *   synchronization and employs an asymmetric chopstick pickup
 *   strategy (philosopher 0 picks up right first, others pick up left
 *   first) to prevent deadlock.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Consts
#define N               5
#define Busy_Eating     1
#define Busy_Thinking   1
#define Left(p)         (p)
#define Right(p)        (((p)+1) % N)

//Typedefs
typedef int* semaphore;

//Prototype Functions
semaphore   make_semaphore(void);
void        philosopher(int me);
void        pick_up(int me);
void        put_down(int me);
void        sem_signal(semaphore s);
void        sem_wait(semaphore s);

//Globals
semaphore chopstick[N];

//MAIN
int main(void)
{
    int i;

    for (i = 0; i < N; i++) {
        chopstick[i] = make_semaphore();
        sem_signal(chopstick[i]);          /* chopstick starts available */
    }

    for (i = 0; i < N-1; i++) {
        if (fork() == 0)                   /* child: break, keep value of i */
            break;
    }

    philosopher(i);                        /* parent: i=N-1, child: i=its own */
    return EXIT_SUCCESS;
}

//Helper-Functions

void pick_up(int me)
{
    if (me == 0) {                         /* philosopher 0 picks up right first */
        sem_wait(chopstick[Right(me)]);
        printf("Philosopher %d picks up right chopstick\n", me);
        sleep(1);
        sem_wait(chopstick[Left(me)]);
        printf("Philosopher %d picks up left chopstick\n", me);
    } else {
        sem_wait(chopstick[Left(me)]);
        printf("Philosopher %d picks up left chopstick\n", me);
        sleep(1);
        sem_wait(chopstick[Right(me)]);
        printf("Philosopher %d picks up right chopstick\n", me);
    }
}

void put_down(int me)
{
    sem_signal(chopstick[Left(me)]);
    sem_signal(chopstick[Right(me)]);
}

void philosopher(int me)
{
    const char *s;
    int i = 1;

    for ( ; ; i++) {
        pick_up(me);
        s = i == 1 ? "st" : i == 2 ? "nd" : i == 3 ? "rd" : "th";
        printf("Philosopher %d eating for the %d%s time\n", me, i, s);
        sleep(Busy_Eating);
        put_down(me);
        printf("Philosopher %d thinking\n", me);
        sleep(Busy_Thinking);
    }
}

semaphore make_semaphore(void)
{
    int *sema = calloc(2, sizeof(int));
    pipe(sema);
    return sema;
}

void sem_wait(semaphore s)
{
    int junk;
    if (read(s[0], &junk, 1) <= 0) {
        fprintf(stderr, "ERROR: sem_wait() failed\n");
        exit(EXIT_FAILURE);
    }
}

void sem_signal(semaphore s)
{
    if (write(s[1], "x", 1) <= 0) {
        fprintf(stderr, "ERROR: sem_signal() failed\n");
        exit(EXIT_FAILURE);
    }
}