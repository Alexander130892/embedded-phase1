/*
 * File:    Ex_07_pipe_capacity.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This program determines the buffer capacity of a Unix pipe by
 *   having a child process write bytes in non-blocking mode until the
 *   pipe becomes full, then reports how many bytes were successfully
 *   written. The parent process waits for the child to complete, then
 *   drains and counts all bytes from the pipe to verify the capacity
 *   measurement.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

//Consts

//Typedefs

//Prototype Functions

//MAIN
int main(void)
{
    int fd[2];

    if (pipe(fd) < 0) {
        perror("pipe failed");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid == 0) {                         /* child: writer */
        close(fd[0]);                       /* close read end */

        /* Set write end to non-blocking */
        int flags = fcntl(fd[1], F_GETFL, 0);
        fcntl(fd[1], F_SETFL, flags | O_NONBLOCK);

        char byte = 'X';
        int count = 0;

        while (write(fd[1], &byte, 1) == 1)
            count++;

        if (errno == EAGAIN)
            printf("Pipe full after %d bytes\n", count);
        else
            perror("write error");

        close(fd[1]);
        exit(EXIT_SUCCESS);
    }

    /* Parent: close write end, wait, then drain pipe */
    close(fd[1]);
    wait(NULL);

    /* Drain so child could theoretically resume (already exited here,
     * but good practice — also lets you verify count via read) */
    char buf[4096];
    ssize_t total = 0, n;
    while ((n = read(fd[0], buf, sizeof(buf))) > 0)
        total += n;

    printf("Parent drained %zd bytes from pipe\n", total);
    close(fd[0]);

    return EXIT_SUCCESS;
}

//Helper-Functions