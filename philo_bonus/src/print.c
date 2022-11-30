#include "philosophers_bonus.h"

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	printf("%5ld %i %s\n", philo_ms_since(start_time), philo_name, str);
}
