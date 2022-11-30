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

int	setup_stop_signal_checker(t_main_data *m)
{
	int			ret;
	pthread_t	thread;

	ret = pthread_create(&thread, NULL, stop_checker, m);
	if (ret)
	{
		printf("ERROR: Failed to create stop signal checking thread\n");
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
	ret = setup_stop_signal_checker(&m);
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
