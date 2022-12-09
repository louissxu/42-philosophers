/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 00:19:32 by lxu               #+#    #+#             */
/*   Updated: 2022/12/10 00:19:34 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int				ignore_err;
	struct timeval	t;

	t = philo_time_now_safe(&ignore_err);
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
