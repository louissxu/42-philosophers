#include "philosophers_bonus.h"

static void	kill_philo_processes(t_main_data *m)
{
	int	i;

	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		kill(m->philo_pid[i], SIGTERM);
		i++;
	}
}

static void	*stop_checker(void *arg)
{
	t_main_data	*m;

	m = (t_main_data *)arg;
	sem_wait(m->sem.stop);
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
	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		sem_wait(m->sem.full);
		i++;
	}
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
