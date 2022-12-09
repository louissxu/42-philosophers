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
		{
			exit_thread = TRUE;
		}
		else if (t->waiter->number_of_full_philosophers >= t->input_data->number_of_philosophers)
		{
			exit_thread = TRUE;
		}
		else if (t->philo->state == 1)
		{
			philo_eating(t);
		}
		else if (t->philo->state == 2)
		{
			philo_sleeping(t);
		}
		else
		{
			philo_thinking(t);
		}
		pthread_mutex_unlock(&t->waiter->lock);
		if (exit_thread)
		{
			pthread_exit(NULL);
		}
		usleep(100);
	}
}

void	free_all(t_main_data *m)
{
	if (m->waiter.fork_in_use)
	{
		free(m->waiter.fork_in_use);
	}
	if (m->waiter.fork_locks)
	{
		free(m->waiter.fork_locks);
	}
	if (m->threads)
	{
		free(m->threads);
	}
	if (m->thread_data)
	{
		free(m->thread_data);
	}
	if (m->philosophers)
	{
		free(m->philosophers);
	}
}

int	main(int argc, char **argv)
{
	t_main_data		m;
	int				i;
	int				rc;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Number of args needs to be 4 or 5\n");
		return (EXIT_SUCCESS);
	}

	// Input data setup
	m.input_data.number_of_philosophers = philo_simple_atoi(argv[1]);
	m.input_data.time_to_die = philo_simple_atoi(argv[2]) * 1000;
	m.input_data.time_to_eat = philo_simple_atoi(argv[3]) * 1000;
	m.input_data.time_to_sleep = philo_simple_atoi(argv[4]) * 1000;
	m.input_data.infinite_simulation = TRUE;
	if (argc == 6)
	{
		m.input_data.infinite_simulation = FALSE;
		m.input_data.number_of_times_each_philosopher_must_eat = philo_simple_atoi(argv[5]);
	}
	if ((rc = gettimeofday(&m.input_data.start_time, NULL)))
	{
		printf("error: gettimeofday, rc: %d\n", rc);
		return (EXIT_FAILURE);
	}

	// Waiter setup
	pthread_mutex_init(&m.waiter.lock, NULL);
	m.waiter.fork_in_use = malloc(sizeof (BOOL) * (m.input_data.number_of_philosophers + 1));
	m.waiter.fork_locks = malloc(sizeof (pthread_mutex_t) * (m.input_data.number_of_philosophers + 1));
	i = 1;
	while (i <= m.input_data.number_of_philosophers)
	{
		m.waiter.fork_in_use[i] = FALSE;
		pthread_mutex_init(&m.waiter.fork_locks[i], NULL);
		i++;
	}
	m.waiter.someone_has_died = FALSE;
	m.waiter.number_of_full_philosophers = 0;

	// Threads and philosophers setup

	// Populate/initialise philosophers
	m.philosophers = malloc(sizeof (t_philosopher_data) * (m.input_data.number_of_philosophers + 1));
	i = 1;
	while (i <= m.input_data.number_of_philosophers)
	{
		m.philosophers[i].id = i;
		m.philosophers[i].fork_r = i;
		m.philosophers[i].fork_l = i - 1;
		if (i == 1)
		{
			m.philosophers[i].fork_l = m.input_data.number_of_philosophers;
		}

		m.philosophers[i].times_eaten = 0;
		m.philosophers[i].state = 3;
		m.philosophers[i].last_ate = m.input_data.start_time;
		
		i++;
	}

	// Create thread data
	m.threads = malloc(sizeof (pthread_t) * (m.input_data.number_of_philosophers + 1));
	m.thread_data = malloc(sizeof (t_thread_data) * (m.input_data.number_of_philosophers + 1));
	pthread_mutex_lock(&m.waiter.lock);
	i = 1;
	while (i <= m.input_data.number_of_philosophers)
	{
		m.thread_data[i].philo = &m.philosophers[i];
		m.thread_data[i].philo_l = &m.philosophers[i - 1];
		if (i == 1)
		{
			m.thread_data[i].philo_l = &m.philosophers[m.input_data.number_of_philosophers];
		}
		m.thread_data[i].philo_r = &m.philosophers[i + 1];
		if (i == m.input_data.number_of_philosophers)
		{
			m.thread_data[i].philo_r = &m.philosophers[1];
		}
		m.thread_data[i].input_data = &m.input_data;
		m.thread_data[i].waiter = &m.waiter;

		if ((rc = pthread_create(&m.threads[i], NULL, philosopher_thread, &m.thread_data[i])))
		{
			printf("error: pthread_create, i: %i  rc: %d\n", i, rc);
			return (EXIT_FAILURE);
		}
		i++;
	}
	pthread_mutex_unlock(&m.waiter.lock);
	// Bind to all the threads
	i = 1;
	while (i <= m.input_data.number_of_philosophers)
	{
		pthread_join(m.threads[i], NULL);
		i++;
	}

	free_all(&m);
	return (EXIT_SUCCESS);
}
