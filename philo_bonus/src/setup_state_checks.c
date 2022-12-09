#include "philosophers_bonus.h"

static void	kill_philo_processes(t_main_data *m)
{
	int	i;

	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		printf("killing process %i\n", i);
		printf("it's pid is at %i\n", m->philo_pid[i]);
		kill(m->philo_pid[i], SIGTERM);
		i++;
	}
	printf("finished killing processes\n");
}

static void	*stop_checker(void *arg)
{
	t_main_data	*m;

	m = (t_main_data *)arg;
	printf("created stop checker, waiting\n");
	sem_wait(m->sem.stop);
	printf("stop checker proceeded\n");
	// sem_wait(m->sem.mutex);
	kill_philo_processes(m);
	// sem_post(m->sem.mutex);
	return (NULL);
}

static void	*full_checker(void *arg)
{
	t_main_data	*m;
	int			i;

	m = (t_main_data *)arg;
	printf("created full checker, waiting\n");
	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		sem_wait(m->sem.full);
		printf("%i people are full\n", i);
		i++;
	}
	printf("everyone is full now, you can stop\n");
	sem_post(m->sem.stop);
	return (NULL);
}

int	setup_state_checks(t_main_data *m)
{
	int			ret;
	pthread_t	stop_check_thread;
	pthread_t	full_check_thread;

	ret = pthread_create(&stop_check_thread, NULL, stop_checker, m);
	if (ret)
	{
		printf("ERROR: Failed to create stop signal checking thread\n");
		return (1);
	}
	ret = pthread_create(&full_check_thread, NULL, full_checker, m);
	if (ret)
	{
		printf("ERROR: Failed to create full checker thread\n");
		return (1);
	}
	return (0);
}
