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

BOOL	setup_mallocs(t_main_data *m)
{
	int	num_cells;

	num_cells = m->input_data.number_of_philosophers + 1;
	m->waiter.fork_in_use = malloc(sizeof (BOOL) * num_cells);
	m->waiter.fork_locks = malloc(sizeof (pthread_mutex_t) * num_cells);
	m->philosophers = malloc(sizeof (t_philosopher_data) * num_cells);
	m->threads = malloc(sizeof (pthread_t) * num_cells);
	m->thread_data = malloc(sizeof (t_thread_data) * num_cells);
	if (!m->waiter.fork_in_use || \
		!m->waiter.fork_locks || \
		!m->philosophers || \
		!m->threads || \
		!m->thread_data)
	{
		return (TRUE);
	}
	return (FALSE);
}

void	setup_waiter(t_main_data *m)
{
	int	i;

	pthread_mutex_init(&m->waiter.lock, NULL);
	i = 1;
	while (i <= m->input_data.number_of_philosophers)
	{
		m->waiter.fork_in_use[i] = FALSE;
		pthread_mutex_init(&m->waiter.fork_locks[i], NULL);
		i++;
	}
	m->waiter.someone_has_died = FALSE;
	m->waiter.number_of_full_philosophers = 0;
}

void	setup_philosophers(t_main_data *m)
{
	int	i;

	i = 1;
	while (i <= m->input_data.number_of_philosophers)
	{
		m->philosophers[i].id = i;
		m->philosophers[i].fork_r = i;
		m->philosophers[i].fork_l = i - 1;
		if (i == 1)
		{
			m->philosophers[i].fork_l = m->input_data.number_of_philosophers;
		}
		m->philosophers[i].times_eaten = 0;
		m->philosophers[i].state = 3;
		m->philosophers[i].last_ate = m->input_data.start_time;
		i++;
	}
}

void	setup_thread_data(t_main_data *m)
{
	int	i;

	i = 1;
	while (i <= m->input_data.number_of_philosophers)
	{
		m->thread_data[i].philo = &m->philosophers[i];
		m->thread_data[i].philo_l = &m->philosophers[i - 1];
		if (i == 1)
		{
			m->thread_data[i].philo_l = \
				&m->philosophers[m->input_data.number_of_philosophers];
		}
		m->thread_data[i].philo_r = &m->philosophers[i + 1];
		if (i == m->input_data.number_of_philosophers)
		{
			m->thread_data[i].philo_r = &m->philosophers[1];
		}
		m->thread_data[i].input_data = &m->input_data;
		m->thread_data[i].waiter = &m->waiter;
		i++;
	}
}

BOOL	setup_threads(t_main_data *m)
{
	int	i;
	int	ret;

	i = 1;
	while (i <= m->input_data.number_of_philosophers)
	{
		ret = pthread_create(&m->threads[i], \
			NULL, philosopher_thread, &m->thread_data[i]);
		if (ret)
		{
			printf("error: pthread_create, i: %i  rc: %d\n", i, ret);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}
