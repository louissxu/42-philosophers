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

#include "philosophers.h"

void	philo_eating(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	if (time_since(t->philo->last_ate) > t->input_data->time_to_die)
	{
		print_line(t->input_data->start_time, t->philo->id, "died");
		t->waiter->someone_has_died = TRUE;
	}
	if (time_since(t->philo->last_ate) < t->input_data->time_to_eat)
	{
		return ;
	}
	pthread_mutex_unlock(&t->waiter->fork_locks[t->philo->fork_l]);
	t->waiter->fork_in_use[t->philo->fork_l] = FALSE;
	pthread_mutex_unlock(&t->waiter->fork_locks[t->philo->fork_r]);
	t->waiter->fork_in_use[t->philo->fork_r] = FALSE;
	print_line(t->input_data->start_time, t->philo->id, "is sleeping");
	t->philo->state = 2;
}

void	philo_sleeping(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	if (time_since(t->philo->last_ate) > t->input_data->time_to_die)
	{
		print_line(t->input_data->start_time, t->philo->id, "died");
		t->waiter->someone_has_died = TRUE;
	}
	if (time_since(t->philo->last_ate) < \
	t->input_data->time_to_eat + t->input_data->time_to_sleep)
	{
		return ;
	}
	print_line(t->input_data->start_time, t->philo->id, "is thinking");
	t->philo->state = 3;
}

BOOL	philo_thinking_early_return_conditions(t_thread_data *t)
{
	if (t->waiter->fork_in_use[t->philo->fork_l] == TRUE || \
		t->waiter->fork_in_use[t->philo->fork_r] == TRUE)
	{
		return (TRUE);
	}
	if (t->philo->fork_l == t->philo->fork_r)
	{
		return (TRUE);
	}
	if ((time_since(t->philo_l->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_l->last_ate) + t->input_data->time_to_eat + \
		STARVING_BUFFER > t->input_data->time_to_die) || \
		(time_since(t->philo_r->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_r->last_ate) + t->input_data->time_to_eat + \
		STARVING_BUFFER > t->input_data->time_to_die))
	{
		return (TRUE);
	}
	return (FALSE);
}

void	philo_thinking(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	if (time_since(t->philo->last_ate) > t->input_data->time_to_die)
	{
		print_line(t->input_data->start_time, t->philo->id, "died");
		t->waiter->someone_has_died = TRUE;
	}
	if (philo_thinking_early_return_conditions(t))
		return ;
	pthread_mutex_lock(&t->waiter->fork_locks[t->philo->fork_l]);
	t->waiter->fork_in_use[t->philo->fork_l] = TRUE;
	print_line(t->input_data->start_time, t->philo->id, "has taken a fork");
	pthread_mutex_lock(&t->waiter->fork_locks[t->philo->fork_r]);
	t->waiter->fork_in_use[t->philo->fork_r] = TRUE;
	print_line(t->input_data->start_time, t->philo->id, "has taken a fork");
	print_line(t->input_data->start_time, t->philo->id, "is eating");
	gettimeofday(&t->philo->last_ate, NULL);
	t->philo->times_eaten += 1;
	if (t->philo->times_eaten >= \
	t->input_data->number_of_times_each_philosopher_must_eat && \
	t->input_data->infinite_simulation == FALSE)
		t->waiter->number_of_full_philosophers++;
	t->philo->state = 1;
}
