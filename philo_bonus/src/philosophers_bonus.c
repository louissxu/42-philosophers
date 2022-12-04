#include "philosophers_bonus.h"

void	wait_for_all_processes_to_exit(t_main_data *m)
{
	int	i;
	int	ignore_ret;

	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		waitpid(m->philo_pid[i], &ignore_ret, 0);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_main_data	m;
	int			ret;

	ret = parse_input_args(argc, argv, &m);
	if (ret)
		return (EXIT_SUCCESS);
	ret = malloc_main_data(&m);
	if (ret)
		return (EXIT_SUCCESS);
	ret = setup_state_checks(&m);
	if (ret)
	{
		free_mallocs(&m);
		return (EXIT_SUCCESS);
	}
	setup_semaphores(&m);
	usleep(1000);
	spawn_philo_processes(&m);
	wait_for_all_processes_to_exit(&m);
	sem_wait(m.sem.mutex);
	close_semaphores(&m);
	free_mallocs(&m);
	return (0);
}
