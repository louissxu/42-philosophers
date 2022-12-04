#include "philosophers_bonus.h"

struct timeval	philo_time_diff(struct timeval t0, struct timeval t1)
{
	struct timeval	t_new;

	t_new.tv_sec = t0.tv_sec - t1.tv_sec;
	t_new.tv_usec = t0.tv_usec - t1.tv_usec;
	t_new = philo_time_normalize(t_new);
	return (t_new);
}

struct timeval	philo_time_since(struct timeval t)
{
	struct timeval	current_time;
	struct timeval	time_since;
	int				err;

	err = FALSE;
	current_time = philo_time_now_safe(&err);
	if (err)
	{
		time_since.tv_sec = 0;
		time_since.tv_usec = 0;
		return (time_since);
	}
	time_since = philo_time_diff(current_time, t);
	return (time_since);
}

long int	philo_time_to_ms(struct timeval t)
{
	long int	result;

	result = t.tv_sec * 1000 + (t.tv_usec / 1000);
	return (result);
}

long int	philo_ms_since(struct timeval t)
{
	return (philo_time_to_ms(philo_time_since(t)));
}

BOOL	philo_time_is_greater_than_zero(struct timeval t)
{
	philo_time_normalize(t);
	if (t.tv_sec > 0)
	{
		return (TRUE);
	}
	if (t.tv_sec == 0 && t.tv_usec > 0)
	{
		return (TRUE);
	}
	return (FALSE);
}
