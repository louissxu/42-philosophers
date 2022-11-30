#include "philosophers_bonus.h"

void	kill_philo_processes(t_main_data *m)
{
	int	i;

	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		kill(m->philo_pid[i], SIGTERM);
		i++;
	}
}

void	*stop_checker(void *arg)
{
	t_main_data	*m;

	m = (t_main_data *)arg;
	printf("created stop checker, waiting\n");
	sem_wait(m->sem.stop);
	printf("stop checker proceeded\n");
	kill_philo_processes(m);
	return (NULL);
}

void	*full_checker(void *arg)
{
	t_main_data	*m;
	int			i;

	m = (t_main_data *)arg;
	printf("created full checker, waiting\n");
	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		printf("%i people are full\n", i);
		sem_wait(m->sem.full);
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

int main(int argc, char **argv)
{
	t_main_data	m;
	int			ret; 

	ret = parse_input_args(argc, argv, &m);
	if (ret)
	{
		return (EXIT_SUCCESS);
	}
	ret = setup_main_data(&m);
	if (ret)
	{
		return (EXIT_SUCCESS);
	}
	ret = setup_state_checks(&m);
	if (ret)
	{
		return (EXIT_SUCCESS);
	}
	setup_semaphores(&m);
	//sem_wait(m.sem.mutex);
	spawn_philo_processes(&m);

	printf("doing the wait\n");
	int i = 1;
	while (i <= m.input.number_of_philosophers)
	{
		waitpid(m.philo_pid[i], &ret, 0);
		i++;
	}
	printf("All pid's have exited. Exiting program.");

	//sem_post(m.sem.mutex);
	// sem_wait(m.sem.stop);
	// printf("exiting\n");
	//tear down (semaphores and mallocs)
	// add conditional teardowns to higher up
}
