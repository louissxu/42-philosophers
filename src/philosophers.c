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

typedef struct s_input_data
{
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int 	time_to_sleep;
	BOOL	infinite_simulation;
	int		number_of_times_each_philosopher_must_eat;
	struct timeval	start_time;
}	t_input_data;

typedef struct s_philosopher_data
{
	int	id;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t *fork_r;
	t_input_data	*input_data;
}	t_philosopher_data;



typedef struct s_main_data
{
	t_input_data		input_data;
	pthread_t			threads[NUM_THREADS];
	pthread_mutex_t		fork_locks[NUM_THREADS];
	t_philosopher_data	philosophers_data[NUM_THREADS];
	// struct timeval		start_time;
}	t_main_data;

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	struct timeval	current_time;
	long int	delta_ms;

	gettimeofday(&current_time, NULL);
	delta_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + (current_time.tv_usec - start_time.tv_usec) / 1000;
	printf("%5ld Philosopher %i is %s\n", delta_ms, philo_name, str);
}

void	*philosopher_thread(void *args)
{
	t_philosopher_data	*philosopher_data;

	philosopher_data = (t_philosopher_data *)args;
	print_line(philosopher_data->input_data->start_time, philosopher_data->id, "thinking (and waiting for forks)");
	pthread_mutex_lock(philosopher_data->fork_r);;
	print_line(philosopher_data->input_data->start_time, philosopher_data->id, "picked up right (1st) fork");
	pthread_mutex_lock(philosopher_data->fork_l);
	print_line(philosopher_data->input_data->start_time, philosopher_data->id, "picked up left (2nd) fork");
	print_line(philosopher_data->input_data->start_time, philosopher_data->id, "eating");
	usleep(philosopher_data->input_data->time_to_eat * 1000);
	pthread_mutex_unlock(philosopher_data->fork_r);
	pthread_mutex_unlock(philosopher_data->fork_l);
	print_line(philosopher_data->input_data->start_time, philosopher_data->id, "done eating and put back the forks");

	pthread_exit(NULL);
	
}

int	philo_simple_atoi(const char *str)
{
	long int	result;
	int			post_mult;
	size_t		i;

	result = 0;
	post_mult = 1;
	i = 0;
	if (str[i] == '-')
	{
		post_mult = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	return (int)(result * post_mult);
}

int	main(int argc, char **argv)
{
	t_main_data		main_data;
	int				i;
	int				rc;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Number of args needs to be 4 or 5\n");
		return (EXIT_SUCCESS);
	}
	main_data.input_data.number_of_philosophers = philo_simple_atoi(argv[1]);
	main_data.input_data.time_to_die = philo_simple_atoi(argv[2]);
	main_data.input_data.time_to_eat = philo_simple_atoi(argv[3]);
	main_data.input_data.time_to_sleep = philo_simple_atoi(argv[4]);
	main_data.input_data.infinite_simulation = TRUE;
	if (argc == 6)
	{
		main_data.input_data.infinite_simulation = FALSE;
		main_data.input_data.number_of_times_each_philosopher_must_eat = philo_simple_atoi(argv[5]);
	}

	if ((rc = gettimeofday(&main_data.input_data.start_time, NULL)))
	{
		printf("error: gettimeofday, rc: %d\n", rc);
		return (EXIT_FAILURE);
	}

	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_mutex_init(&main_data.fork_locks[i], NULL);
		i++;
	}

	i = 0;
	while (i < NUM_THREADS)
	{
		main_data.philosophers_data[i].id = i + 1;
		main_data.philosophers_data[i].input_data = &main_data.input_data;
		if (i == 0)
		{
			main_data.philosophers_data[i].fork_r = &main_data.fork_locks[NUM_THREADS - 1];
			main_data.philosophers_data[i].fork_l = &main_data.fork_locks[i];
		}
		else
		{
			main_data.philosophers_data[i].fork_r = &main_data.fork_locks[i];
			main_data.philosophers_data[i].fork_l = &main_data.fork_locks[i - 1];
		}
		if ((rc = pthread_create(&main_data.threads[i], NULL, philosopher_thread, &main_data.philosophers_data[i])))
		{
			printf("error: pthread_create, i: %i  rc: %d\n", i, rc);
			return (EXIT_FAILURE);
		}
		i++;
	}
	printf("starting program\n");
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(main_data.threads[i], NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}
