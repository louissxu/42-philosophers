#include "philosophers_bonus.h"

struct timeval	philo_time_create(time_t sec, long int usec)
{
	struct timeval	t;

	t.tv_sec = sec;
	t.tv_usec = usec;
	return (t);
}

struct timeval	philo_time_now_safe(int *err)
{
	struct timeval	t;
	int				ret;

	ret = gettimeofday(&t, NULL);
	if (ret)
	{
		t.tv_sec = 0;
		t.tv_usec = 0;
		*err = TRUE;
	}
	return (t);
}

struct timeval	philo_time_now(void)
{
	int				_err;
	struct timeval	t;

	t = philo_time_now_safe(&_err);
	return (t);
}

struct timeval	philo_time_normalize(struct timeval t)
{
	while (t.tv_usec < 0)
	{
		t.tv_sec--;
		t.tv_usec += 1000000;
	}	
	t.tv_sec += t.tv_usec / 1000000;
	t.tv_usec = t.tv_usec % 1000000;
	return (t);
}

struct timeval	philo_time_add(struct timeval t0, struct timeval t1)
{
	struct timeval	t_new;

	t0 = philo_time_normalize(t0);
	t1 = philo_time_normalize(t1);
	t_new.tv_sec = t0.tv_sec + t1.tv_sec;
	t_new.tv_usec = t0.tv_usec + t1.tv_usec;
	t_new.tv_sec += t_new.tv_usec / 1000000;
	t_new.tv_usec = t_new.tv_usec % 1000000;
	return (t_new);
}

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
	// struct timeval	current_time;
	// int				err;

	// err = FALSE;
	// current_time = philo_time_now_safe(&err);
	// if (err)
	// {
	// 	return (0);
	// }
	// return (philo_time_to_ms(philo_time_diff(current_time, t)));
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
