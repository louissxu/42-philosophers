#include "philosophers_bonus.h"

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
	setup_semaphores(&m);
	spawn_philo_processes(&m);
	// sem_wait(m.sem.stop);
	// printf("exiting\n");
	//tear down
}
