#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define SEM "user-sem1"

typedef struct s_sem_holder
{
	sem_t	*sem;
}	t_sem_holder;

int internal_routine(sem_t *sem)
{
	// sem_t *sem;
	// sem = sem_open(SEM, 0);
	printf("child: starting\n");
	printf("child: waiting for lock\n");
	sem_wait(sem);
	printf("child: has lock. beginning 2 second wait\n");
	usleep(2000000);
	printf("child: finished 2 second wait. releasing lock\n");
	sem_post(sem);

	return (0);
}

int spawn_child(sem_t *sem)
{
	int pid = fork();
	if (pid)
	{
		return (pid);
	}
	// sem_t *sem = sem_open(SEM, 0);
	internal_routine(sem);
	
	exit(0);
}


int main(void)
{
	sem_t *sem;
	sem = sem_open(SEM, 0);
	sem_close(sem);
	sem_unlink(SEM);

	sem = sem_open(SEM, O_CREAT | O_EXCL, 0644, 1);

	printf("parent: hello world. program starting\n");
	printf("parent: spawning children x2\n");
	int ret = spawn_child(sem);
	printf("parent: first pid is %i\n", ret);
	ret = spawn_child(sem);
	printf("parent: second pid is %i\n", ret);
	printf("parent: waiting for lock\n");
	sem_wait(sem);
	printf("parent: has lock. starting 5 second wait\n");
	usleep(5000000);
	printf("parent: finished 5 second wait. releasing lock\n");
	sem_post(sem);
	printf("parent: starting 10 second wait.\n");
	usleep(10000000);
	printf("parent: program has finished!\n");
	return (0);
}