#include "philosophers.h"

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	printf("%5ld %i %s\n", time_since(start_time) / 1000, philo_name, str);
}

void	print_took_fork(struct timeval start_time, int philo_name, int fork_num)
{
	printf("%5ld Philosopher %i has taken fork number %i\n", \
		time_since(start_time), philo_name, fork_num);
}
