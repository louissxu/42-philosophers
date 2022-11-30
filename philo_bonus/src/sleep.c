#include "philosophers_bonus.h"

void	ft_timeval_sleep(struct timeval t)
{
	usleep((t.tv_sec * 1000000) + t.tv_usec);
}
