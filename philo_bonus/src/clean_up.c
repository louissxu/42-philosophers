#include "philosophers_bonus.h"

void	close_semaphores(t_main_data *m)
{
	sem_close(m->sem.mutex);
	sem_unlink(SEM_MUTEX);
	sem_close(m->sem.chopsticks);
	sem_unlink(SEM_CHOPSTICKS);
	sem_close(m->sem.full);
	sem_unlink(SEM_FULL);
	sem_close(m->sem.stop);
	sem_unlink(SEM_STOP);
}

void	free_mallocs(t_main_data *m)
{
	free(m->philo_pid);
}