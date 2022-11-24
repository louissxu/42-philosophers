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

typedef struct	s_waiter_data
{
	pthread_mutex_t lock;
	BOOL			*fork_in_use;
	pthread_mutex_t	*fork_locks;
	BOOL			someone_has_died;
}	t_waiter_data;

typedef struct s_philosopher_data
{
	int	id;
	int	fork_l;
	int fork_r;
	struct s_philosopher_data	*p_l;
	struct s_philosopher_data	*p_r;
	int times_eaten;
	struct timeval	last_ate;
	t_input_data	*input_data;
	t_waiter_data *waiter;
}	t_philosopher_data;

typedef struct s_main_data
{
	t_input_data		input_data;
	pthread_t			*threads;
	t_philosopher_data	*philosophers_data;
	t_waiter_data		waiter;
	// struct timeval		start_time;
}	t_main_data;

long int	time_delta_ms(struct timeval initial, struct timeval final)
{
	long int	delta_ms;

	delta_ms = (final.tv_sec - initial.tv_sec) * 1000 + (final.tv_usec - initial.tv_usec) / 1000;
	return (delta_ms);
}

long int	time_since(struct timeval t)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (time_delta_ms(t, current_time));
}

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	printf("%5ld Philosopher %i %s\n", time_since(start_time), philo_name, str);
}

void	print_took_fork(struct timeval start_time, int philo_name, int fork_num)
{
	printf("%5ld Philosopher %i has taken fork number %i\n", time_since(start_time), philo_name, fork_num);
}

BOOL	philo_try_eat(void *args)
{
	t_philosopher_data	*p;

	p = (t_philosopher_data *)args;
	pthread_mutex_lock(&p->waiter->lock);
	if (p->waiter->someone_has_died)
	{
		pthread_mutex_unlock(&p->waiter->lock);
		pthread_exit(NULL);
	}
	if (time_since(p->last_ate) > p->input_data->time_to_die)
	{
		print_line(p->input_data->start_time, p->id, "died");
		p->waiter->someone_has_died = TRUE;
		pthread_mutex_unlock(&p->waiter->lock);
		pthread_exit(NULL);
	}
	if (time_since(p->p_l->last_ate) > time_since(p->last_ate) || time_since(p->p_r->last_ate) > time_since(p->last_ate))
	{
		if (time_since(p->p_l->last_ate) + p->input_data->time_to_eat + 5 > p->input_data->time_to_die || \
			time_since(p->p_r->last_ate) + p->input_data->time_to_eat + 5 > p->input_data->time_to_die)
		{
			// print_line(p->input_data->start_time, p->id, "unlocking without success");
			pthread_mutex_unlock(&p->waiter->lock);
			return (FALSE);
		}
	}
	if (p->waiter->fork_in_use[p->fork_l] == TRUE || p->waiter->fork_in_use[p->fork_r] == TRUE)
	{
		// print_line(p->input_data->start_time, p->id, "unlocking without success");
		pthread_mutex_unlock(&p->waiter->lock);
		return (FALSE);
	}
	// print_line(p->input_data->start_time, p->id, "has locked the waiter");
	pthread_mutex_lock(&p->waiter->fork_locks[p->fork_l]);
	p->waiter->fork_in_use[p->fork_l] = TRUE;
	// print_took_fork(p->input_data->start_time, p->id, p->fork_l);
	pthread_mutex_lock(&p->waiter->fork_locks[p->fork_r]);
	p->waiter->fork_in_use[p->fork_r] = TRUE;
	// print_took_fork(p->input_data->start_time, p->id, p->fork_r);
	print_line(p->input_data->start_time, p->id, "is eating");
	// print_line(p->input_data->start_time, p->id, "is unlocking the waiter");
	pthread_mutex_unlock(&p->waiter->lock);


	gettimeofday(&p->last_ate, NULL);
	p->times_eaten += 1;
	usleep(p->input_data->time_to_eat * 1000);
	pthread_mutex_lock(&p->waiter->lock);
	pthread_mutex_unlock(&p->waiter->fork_locks[p->fork_l]);
	p->waiter->fork_in_use[p->fork_l] = FALSE;
	pthread_mutex_unlock(&p->waiter->fork_locks[p->fork_r]);
	p->waiter->fork_in_use[p->fork_r] = FALSE;
	pthread_mutex_unlock(&p->waiter->lock);
	return (TRUE);
}

void	philo_eat(void *args)
{
	t_philosopher_data	*p;
	BOOL				ate_successfully;

	p = (t_philosopher_data *)args;
	ate_successfully = FALSE;
	while (ate_successfully == FALSE)
	{
		// usleep(1); // Necessary?
		ate_successfully = philo_try_eat(p);
	}
}

void	philo_sleep(void *args)
{
	t_philosopher_data	*p;

	p = (t_philosopher_data *)args;
	if (p->waiter->someone_has_died)
	{
		pthread_exit(NULL);
	}
	print_line(p->input_data->start_time, p->id, "is sleeping");
	usleep(p->input_data->time_to_sleep * 1000);
}

void	philo_think(void *args)
{
	t_philosopher_data	*p;

	p = (t_philosopher_data *)args;
	if (p->waiter->someone_has_died)
	{
		pthread_exit(NULL);
	}
	print_line(p->input_data->start_time, p->id, "is thinking");
}

void	*philosopher_thread(void *args)
{
	t_philosopher_data	*p;

	p = (t_philosopher_data *)args;
	while (TRUE)
	{
		if (p->times_eaten >= p->input_data->number_of_times_each_philosopher_must_eat && p->input_data->infinite_simulation == FALSE)
		{
			pthread_exit(NULL);
		}
		philo_eat(p);
		if (p->times_eaten >= p->input_data->number_of_times_each_philosopher_must_eat && p->input_data->infinite_simulation == FALSE)
		{
			pthread_exit(NULL);
		}
		philo_sleep(p);
		philo_think(p);
	}
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
	while (str[i] >= '0' & str[i] <= '9')
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
	pthread_mutex_init(&main_data.waiter.lock, NULL);
	main_data.waiter.fork_in_use = malloc(sizeof (BOOL) * main_data.input_data.number_of_philosophers);
	main_data.waiter.fork_locks = malloc(sizeof (pthread_mutex_t) * main_data.input_data.number_of_philosophers);
	main_data.threads = malloc(sizeof (pthread_t) * main_data.input_data.number_of_philosophers);
	main_data.philosophers_data = malloc(sizeof (t_philosopher_data) * main_data.input_data.number_of_philosophers);

	main_data.waiter.someone_has_died = FALSE;
	if ((rc = gettimeofday(&main_data.input_data.start_time, NULL)))
	{
		printf("error: gettimeofday, rc: %d\n", rc);
		return (EXIT_FAILURE);
	}

	i = 0;
	while (i < main_data.input_data.number_of_philosophers)
	{
		pthread_mutex_init(&main_data.waiter.fork_locks[i], NULL);
		i++;
	}

	i = 0;
	pthread_mutex_lock(&main_data.waiter.lock);

	while (i < main_data.input_data.number_of_philosophers)
	{
		main_data.philosophers_data[i].id = i;
		if (i == 0)
		{
			main_data.philosophers_data[i].fork_r = i;
			main_data.philosophers_data[i].fork_l = main_data.input_data.number_of_philosophers - 1;
		}
		else
		{
			main_data.philosophers_data[i].fork_r = i;
			main_data.philosophers_data[i].fork_l = i - 1;
		}
		main_data.philosophers_data[i].times_eaten = 0;
		main_data.philosophers_data[i].last_ate = main_data.input_data.start_time;
		main_data.philosophers_data[i].input_data = &main_data.input_data;
		main_data.philosophers_data[i].waiter = &main_data.waiter;
		
		if (i == 0)
		{
			main_data.philosophers_data[i].p_r = &main_data.philosophers_data[i + 1];
			main_data.philosophers_data[i].p_l = &main_data.philosophers_data[main_data.input_data.number_of_philosophers - 1];
		}
		else if (i == main_data.input_data.number_of_philosophers - 1)
		{
			main_data.philosophers_data[i].p_r = &main_data.philosophers_data[0];
			main_data.philosophers_data[i].p_l = &main_data.philosophers_data[i - 1];
		}
		else
		{
			main_data.philosophers_data[i].p_r = &main_data.philosophers_data[i + 1];
			main_data.philosophers_data[i].p_l = &main_data.philosophers_data[i - 1];
		}

		if ((rc = pthread_create(&main_data.threads[i], NULL, philosopher_thread, &main_data.philosophers_data[i])))
		{
			printf("error: pthread_create, i: %i  rc: %d\n", i, rc);
			return (EXIT_FAILURE);
		}
		i++;
	}
	printf("starting program\n");
	pthread_mutex_unlock(&main_data.waiter.lock);
	i = 0;
	while (i < main_data.input_data.number_of_philosophers)
	{
		pthread_join(main_data.threads[i], NULL);
		i++;
	}

	free(main_data.waiter.fork_in_use);
	free(main_data.waiter.fork_locks);
	free(main_data.threads);
	free(main_data.philosophers_data);
	return (EXIT_SUCCESS);
}
