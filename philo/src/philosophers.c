/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:00:32 by lxu               #+#    #+#             */
/*   Updated: 2022/11/22 15:00:34 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Ref: https://randu.org/tutorials/threads/

void	*philosopher_thread(void *args)
{
	t_thread_data	*t;
	BOOL			exit_thread;

	t = (t_thread_data *)args;
	exit_thread = FALSE;
	while (TRUE)
	{
		pthread_mutex_lock(&t->waiter->lock);
		if (t->waiter->someone_has_died)
			exit_thread = TRUE;
		else if (t->waiter->number_of_full_philosophers >= \
			t->input_data->number_of_philosophers)
		{
			exit_thread = TRUE;
		}
		else if (t->philo->state == 1)
			philo_eating(t);
		else if (t->philo->state == 2)
			philo_sleeping(t);
		else
			philo_thinking(t);
		pthread_mutex_unlock(&t->waiter->lock);
		if (exit_thread)
			pthread_exit(NULL);
		usleep(100);
	}
}

void	bind_to_all_threads(t_main_data *m)
{
	int	i;

	i = 1;
	while (i <= m->input_data.number_of_philosophers)
	{
		pthread_join(m->threads[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_main_data		m;
	BOOL			error;

	error = parse_args(argc, argv, &m);
	if (error)
	{
		return (EXIT_SUCCESS);
	}
	error = setup_mallocs(&m);
	if (error)
	{
		free_all(&m);
		return (EXIT_SUCCESS);
	}
	setup_waiter(&m);
	setup_philosophers(&m);
	setup_thread_data(&m);
	pthread_mutex_lock(&m.waiter.lock);
	error = setup_threads(&m);
	if (error)
	{
		free_all(&m);
		return (EXIT_SUCCESS);
	}
	pthread_mutex_unlock(&m.waiter.lock);
	bind_to_all_threads(&m);
	free_all(&m);
	return (EXIT_SUCCESS);
}
