#include "philosophers_bonus.h"

void	free_mallocs(t_main_data *m)
{
	free(m->philo_pid);
}