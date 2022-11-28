#include <stdio.h>
#include <unistd.h>

int spawn(void)
{
    printf("running spawn\n");

    int pid = fork();
    if (pid)
    {
        printf("this\n");
    }
    else
    {
        printf("that\n");
    }

    printf("done running spawn\n");
    return (0);
}

int main(void)
{
    printf("starting main\n");
    spawn();
    printf("finishing main\n");
    return (0);
}