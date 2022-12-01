#include "philosophers_bonus.h"

void	clean_up_zombie_semaphores(void)
{
	sem_t	*s_ptr;
	int		i;
	char	*sem_names[4];
	char	*s_name;

	sem_names[0] = SEM_MUTEX;
	sem_names[1] = SEM_CHOPSTICKS;
	sem_names[2] = SEM_FULL;
	sem_names[3] = SEM_STOP;
	i = 0;
	while (i < 4)
	{
		s_name = sem_names[i];
		s_ptr = sem_open(s_name, 0);
		sem_close(s_ptr);
		sem_unlink(s_name);
		i++;
	}
}

void	setup_semaphores(t_main_data *m)
{
	clean_up_zombie_semaphores();
	m->sem.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0600, 1);
	m->sem.chopsticks = sem_open(SEM_CHOPSTICKS, O_CREAT | O_EXCL, 0600, m->input.number_of_philosophers / 2);
	m->sem.full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0600, 0);
	m->sem.stop = sem_open(SEM_STOP, O_CREAT | O_EXCL, 0600, 0);
}

void	open_semaphores(t_semaphores *sem)
{
	sem->mutex = sem_open(SEM_MUTEX, 0);
	sem->chopsticks = sem_open(SEM_CHOPSTICKS, 0);
	sem->full = sem_open(SEM_CHOPSTICKS, 0);
	sem->stop = sem_open(SEM_CHOPSTICKS, 0);
}