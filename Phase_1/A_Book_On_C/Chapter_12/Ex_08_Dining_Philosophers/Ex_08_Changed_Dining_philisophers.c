/*
 * File:    Ex_08_Changed_Dining_philisophers.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This C program implements a modified version of the classic dining
 *   philosophers problem using 5 philosophers as separate processes,
 *   where chopsticks are represented by semaphores and philosopher 3
 *   uses a different chopstick pickup order to avoid deadlock. The
 *   program handles SIGINT to collect and display statistics on how
 *   many times each philosopher ate before termination.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

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
void        sigint_handler(int sig);
void        sigusr1_handler(int sig);

//Globals
semaphore   chopstick[N];
pid_t       children[N-1];
int         report_pipes[N-1][2];

volatile sig_atomic_t my_eat_count = 0;
int my_report_fd = -1;

//MAIN
int main(void)
{
    for (int i = 0; i < N; i++) {
        chopstick[i] = make_semaphore();
        sem_signal(chopstick[i]);
    }

    for (int i = 0; i < N-1; i++) {
        if (pipe(report_pipes[i]) < 0) {
            perror("pipe failed");
            return EXIT_FAILURE;
        }
    }

    struct sigaction sa = {0};
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    int i;
    for (i = 0; i < N-1; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            /* ── child setup ── */

            /* Ignore Ctrl+C — only parent drives shutdown */
            signal(SIGINT, SIG_IGN);

            /* Install SIGUSR1 report handler */
            struct sigaction sc = {0};
            sc.sa_handler = sigusr1_handler;
            sigaction(SIGUSR1, &sc, NULL);

            for (int j = 0; j < N-1; j++) {
                close(report_pipes[j][0]);
                if (j != i)
                    close(report_pipes[j][1]);
            }
            my_report_fd = report_pipes[i][1];
            break;
        }
        children[i] = pid;
        close(report_pipes[i][1]);
    }

    philosopher(i);
    return EXIT_SUCCESS;
}

//Helper-Functions

void sigint_handler(int sig)
{
    (void)sig;

    for (int i = 0; i < N-1; i++)
        kill(children[i], SIGSTOP);

    for (int i = 0; i < N-1; i++)
        kill(children[i], SIGUSR1);

    for (int i = 0; i < N-1; i++)
        kill(children[i], SIGCONT);

    sleep(1);

    printf("\n--- Interrupt received ---\n");

    /* Children 0..N-2 report via pipes */
    for (int i = 0; i < N-1; i++) {
        int count = 0;
        read(report_pipes[i][0], &count, sizeof(count));
        printf("Philosopher %d ate %d time(s)\n", i, count);
    }

    /* Parent IS philosopher N-1 — report directly */
    printf("Philosopher %d ate %d time(s)\n", N-1, (int)my_eat_count);

    for (int i = 0; i < N-1; i++)
        kill(children[i], SIGTERM);
    for (int i = 0; i < N-1; i++)
        wait(NULL);

    exit(EXIT_SUCCESS);
}

void sigusr1_handler(int sig)
{
    (void)sig;
    int count = my_eat_count;
    write(my_report_fd, &count, sizeof(count));
}

void pick_up(int me)
{
    if (me == 3) {
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
        my_eat_count++;
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
    ssize_t r;
    /* SIGUSR1 can interrupt read() — retry on EINTR */
    do {
        r = read(s[0], &junk, 1);
    } while (r < 0 && errno == EINTR);

    if (r <= 0) {
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