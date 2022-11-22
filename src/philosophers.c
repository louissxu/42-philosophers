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

#define NUM_THREADS 5

typedef struct s_thread_data
{
	int		thread_id;
	double	value;
	double	value2;
}	t_thread_data;

typedef struct s_philosopher
{
	int	id;
	pthread_mutex_t	*fork_locks;
}	t_philosopher;

typedef struct s_data
{
	pthread_mutex_t	fork_locks[NUM_THREADS];
	t_philosopher	philosophers[NUM_THREADS];

}	t_data;

double global_shared_x;
pthread_mutex_t lock_x;

void	*philosopher_thread(void *args)
{
	t_philosopher	*philosopher_data;

	philosopher_data = (philosopher_data *)args;
	printf("Philosopher %i is running\n", philosopher_data->id)
	
}

void	*thread_function(void *args)
{
	t_thread_data	*data;

	data = (t_thread_data *)args;
	printf("Running thread. Thread number %i\n", data->thread_id);
	pthread_mutex_lock(&lock_x);
	global_shared_x += 10;
	printf("The current x value is: %f\n", global_shared_x);
	pthread_mutex_unlock(&lock_x);

	pthread_exit(NULL);
}

int	main(int argc, char **argv)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	// pthread_mutex_t	fork_locks[NUM_THREADS];
	int				i;
	int				rc;

	argc++;
	argv++;

	global_shared_x = 11;
	pthread_mutex_init(&lock_x, NULL);

	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].thread_id = i;
		if ((rc = pthread_create(&threads[i], NULL, thread_function, &thread_data[i])))
		{
			printf("error: pthread_create, rc: %d\n", rc);
			return (EXIT_FAILURE);
		}
		printf("std: rc is: %d\n", rc);
		i++;
	}
	printf("starting program");
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}
