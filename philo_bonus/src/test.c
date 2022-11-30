#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int spawn_child(void)
{
    int pid = fork();
    if (pid)
    {
        printf("spawned pid: %i\n", pid);
        return (pid);
    }
    printf("printing from child process\n");
    sem_t *sem = sem_open("b", 0);
    sem_wait(sem);
    printf("starting internal child process\n");
	usleep(1000000);
    printf("exiting child process\n");
    sem_post(sem);
    exit(0);
}

int main(void)
{
    printf("hello world\n");

    printf("o_creat: %i\n", O_CREAT);
    printf("o_excl: %i\n", O_EXCL);
    int errno = 0;
    sem_t   *sem = sem_open("b", O_CREAT | O_EXCL, 0644, 1);
    printf("errno is %i\n", errno);
    // sem_t   *sem =  NULL;

    sem_wait(sem);
    spawn_child();
    spawn_child();
    printf("starting 5 second wait\n");
    usleep(5000000);
    printf("finished 5 second wait\n");
    sem_post(sem);
    printf("finished\n");

    while(1)
    {
        usleep(100);
    }
    return (0);
}