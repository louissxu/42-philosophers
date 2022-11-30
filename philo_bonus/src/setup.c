#include "philosophers_bonus.h"

void	setup_semaphores(t_main_data *m)
{
	// m->sem.mutex = sem_open(SEM_MUTEX, 0);
	// sem_close(m->sem.mutex);
	// sem_unlink(SEM_MUTEX);

	int errno = 0;
	m->sem.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0600, 1);
	m->sem.chopsticks = sem_open(SEM_CHOPSTICKS, O_CREAT | O_EXCL, 0600, m->input.number_of_philosophers);
	m->sem.full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0600, 0);
	m->sem.stop = sem_open(SEM_STOP, O_CREAT | O_EXCL, 0600, 0);
	printf("errno is %i\n", errno);
}

void	open_semaphores(t_semaphores *sem)
{
	int errno = 0;
	sem->mutex = sem_open(SEM_MUTEX, 0);
	sem->chopsticks = sem_open(SEM_CHOPSTICKS, 0);
	sem->full = sem_open(SEM_CHOPSTICKS, 0);
	sem->stop = sem_open(SEM_CHOPSTICKS, 0);
	printf("errno is %i\n", errno);
}