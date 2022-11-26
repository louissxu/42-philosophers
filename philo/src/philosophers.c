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

# define STARVING_BUFFER 1000

/**
 * @var	t_input_data::time_to_die
*/
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
	int				number_of_full_philosophers;
}	t_waiter_data;

/**
 * @struct t_philosopher_data
 * 
 * @var t_philosopher_data::state
 *	The state of the philosopher
 *	1 eating
 *	2 sleeping
 *	3 thinking
 *	
*/
typedef struct	s_philosopher_data
{
	int	id;
	int	fork_l;
	int	fork_r;
	int times_eaten;
	int	state;
	struct timeval	last_ate;
}	t_philosopher_data;

typedef struct	s_thread_data
{
	t_philosopher_data	*philo;
	t_philosopher_data	*philo_l;
	t_philosopher_data	*philo_r;
	t_input_data		*input_data;
	t_waiter_data		*waiter;
}	t_thread_data;

// typedef struct s_philosopher_data
// {
// 	int	id;
// 	int	fork_l;
// 	int fork_r;
// 	struct s_philosopher_data	*p_l;
// 	struct s_philosopher_data	*p_r;
// 	int times_eaten;
// 	struct timeval	last_ate;
// 	t_input_data	*input_data;
// 	t_waiter_data *waiter;
// }	t_philosopher_data;

// typedef struct s_main_data
// {
// 	t_input_data		input_data;
// 	pthread_t			*threads;
// 	t_philosopher_data	*philosophers_data;
// 	t_waiter_data		waiter;
// 	// struct timeval		start_time;
// }	t_main_data;

typedef struct	s_main_data
{
	t_input_data	input_data;
	t_waiter_data	waiter;
	pthread_t		*threads;
	t_thread_data	*thread_data;
	t_philosopher_data	*philosophers;
}	t_main_data;

//REF: https://stackoverflow.com/a/4846569/9160572
long int	time_delta(struct timeval initial, struct timeval final)
{
	long int	delta_ms;

	delta_ms = (final.tv_sec - initial.tv_sec) * 1000000 + (final.tv_usec - initial.tv_usec);
	return (delta_ms);
}

long int	time_since(struct timeval t)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (time_delta(t, current_time));
}

// long int	time_since(struct timeval t)
// {
// 	struct timeval	current_time;
// 	long int		delta_ms;

// 	gettimeofday(&current_time, NULL);
// 	delta_ms = (current_time.tv_sec - t.tv_sec) * 1000 + ((current_time.tv_usec - t.tv_usec) / 1000);
// 	return (delta_ms);
// }

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	printf("%5ld %i %s\n", time_since(start_time) / 1000, philo_name, str);
}

void	print_took_fork(struct timeval start_time, int philo_name, int fork_num)
{
	printf("%5ld Philosopher %i has taken fork number %i\n", time_since(start_time), philo_name, fork_num);
}

BOOL	philo_try_eat(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	pthread_mutex_lock(&t->waiter->lock);
	if (t->waiter->someone_has_died)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		pthread_exit(NULL);
		return (FALSE); // Probably not needed as thread will terminate
	}
	if (time_since(t->philo->last_ate) > t->input_data->time_to_die)
	{
		print_line(t->input_data->start_time, t->philo->id, "died");
		t->waiter->someone_has_died = TRUE;
		pthread_mutex_unlock(&t->waiter->lock);
		pthread_exit(NULL);
		return (FALSE); // Probably not needed
	}
	// If forks are in use
	if (t->waiter->fork_in_use[t->philo->fork_l] == TRUE || t->waiter->fork_in_use[t->philo->fork_r] == TRUE)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return (FALSE);
	}
	if (t->philo->fork_l == t->philo->fork_r)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return (FALSE);
	}

	// If either neighbour is hungrier and one cycle away from dying
	if ((time_since(t->philo_l->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_l->last_ate) + t->input_data->time_to_eat + STARVING_BUFFER > t->input_data->time_to_die) || \
		(time_since(t->philo_r->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_r->last_ate) + t->input_data->time_to_eat + STARVING_BUFFER > t->input_data->time_to_die))
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return (FALSE);
	}

	pthread_mutex_lock(&t->waiter->fork_locks[t->philo->fork_l]);
	t->waiter->fork_in_use[t->philo->fork_l] = TRUE;
	print_line(t->input_data->start_time, t->philo->id, "has taken a fork");
	pthread_mutex_lock(&t->waiter->fork_locks[t->philo->fork_r]);
	t->waiter->fork_in_use[t->philo->fork_r] = TRUE;
	print_line(t->input_data->start_time, t->philo->id, "has taken a fork");
	print_line(t->input_data->start_time, t->philo->id, "is eating");
	pthread_mutex_unlock(&t->waiter->lock);


	gettimeofday(&t->philo->last_ate, NULL);
	t->philo->times_eaten += 1;
	usleep(t->input_data->time_to_eat * 1000);

	pthread_mutex_lock(&t->waiter->lock);
	pthread_mutex_unlock(&t->waiter->fork_locks[t->philo->fork_l]);
	t->waiter->fork_in_use[t->philo->fork_l] = FALSE;
	pthread_mutex_unlock(&t->waiter->fork_locks[t->philo->fork_r]);
	t->waiter->fork_in_use[t->philo->fork_r] = FALSE;
	pthread_mutex_unlock(&t->waiter->lock);
	return (TRUE);
}

void	philo_eating(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	pthread_mutex_lock(&t->waiter->lock);
	if (time_since(t->philo->last_ate) > t->input_data->time_to_die)
	{
		print_line(t->input_data->start_time, t->philo->id, "died");
		t->waiter->someone_has_died = TRUE;
		pthread_mutex_unlock(&t->waiter->lock);
		pthread_exit(NULL);
	}
	if (time_since(t->philo->last_ate) < t->input_data->time_to_eat)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return ;
	}

	pthread_mutex_unlock(&t->waiter->fork_locks[t->philo->fork_l]);
	t->waiter->fork_in_use[t->philo->fork_l] = FALSE;
	pthread_mutex_unlock(&t->waiter->fork_locks[t->philo->fork_r]);
	t->waiter->fork_in_use[t->philo->fork_r] = FALSE;
	print_line(t->input_data->start_time, t->philo->id, "is sleeping");
	t->philo->state = 2;
	pthread_mutex_unlock(&t->waiter->lock);
}

void	philo_eat(void *args)
{
	t_thread_data	*t;
	BOOL				ate_successfully;

	t = (t_thread_data *)args;
	ate_successfully = FALSE;
	while (ate_successfully == FALSE)
	{
		// usleep(1); // Necessary?
		ate_successfully = philo_try_eat(t);
	}
}

void	philo_sleeping(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;

	pthread_mutex_lock(&t->waiter->lock);
	if (time_since(t->philo->last_ate) > t->input_data->time_to_die)
	{
		print_line(t->input_data->start_time, t->philo->id, "died");
		t->waiter->someone_has_died = TRUE;
		pthread_mutex_unlock(&t->waiter->lock);
		pthread_exit(NULL);
	}
	if (time_since(t->philo->last_ate) < t->input_data->time_to_eat + t->input_data->time_to_sleep)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return ;
	}
	print_line(t->input_data->start_time, t->philo->id, "is thinking");
	t->philo->state = 3;
	pthread_mutex_unlock(&t->waiter->lock);
}

void	philo_sleep(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	pthread_mutex_lock(&t->waiter->lock);
	if (t->waiter->someone_has_died)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		pthread_exit(NULL);
	}
	print_line(t->input_data->start_time, t->philo->id, "is sleeping");
	pthread_mutex_unlock(&t->waiter->lock);
	usleep(t->input_data->time_to_sleep * 1000);
}

void	philo_thinking(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	pthread_mutex_lock(&t->waiter->lock);
	if (time_since(t->philo->last_ate) > t->input_data->time_to_die)
	{
		print_line(t->input_data->start_time, t->philo->id, "died");
		t->waiter->someone_has_died = TRUE;
		pthread_mutex_unlock(&t->waiter->lock);
		pthread_exit(NULL);
	}
	// If forks are in use
	if (t->waiter->fork_in_use[t->philo->fork_l] == TRUE || t->waiter->fork_in_use[t->philo->fork_r] == TRUE)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return ;
	}
	// If the forks are the same fork
	if (t->philo->fork_l == t->philo->fork_r)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return ;
	}
	// If either neighbour is hungrier and one cycle away from dying
	if ((time_since(t->philo_l->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_l->last_ate) + t->input_data->time_to_eat + STARVING_BUFFER > t->input_data->time_to_die) || \
		(time_since(t->philo_r->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_r->last_ate) + t->input_data->time_to_eat + STARVING_BUFFER > t->input_data->time_to_die))
	{
		pthread_mutex_unlock(&t->waiter->lock);
		return ;
	}

	pthread_mutex_lock(&t->waiter->fork_locks[t->philo->fork_l]);
	t->waiter->fork_in_use[t->philo->fork_l] = TRUE;
	print_line(t->input_data->start_time, t->philo->id, "has taken a fork");
	pthread_mutex_lock(&t->waiter->fork_locks[t->philo->fork_r]);
	t->waiter->fork_in_use[t->philo->fork_r] = TRUE;
	print_line(t->input_data->start_time, t->philo->id, "has taken a fork");
	print_line(t->input_data->start_time, t->philo->id, "is eating");
	gettimeofday(&t->philo->last_ate, NULL);
	t->philo->times_eaten += 1;
	if (t->philo->times_eaten >= t->input_data->number_of_times_each_philosopher_must_eat && t->input_data->infinite_simulation == FALSE)
	{
		t->waiter->number_of_full_philosophers++;
	}
	t->philo->state = 1;
	pthread_mutex_unlock(&t->waiter->lock);
}

void	philo_think(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	pthread_mutex_lock(&t->waiter->lock);
	if (t->waiter->someone_has_died)
	{
		pthread_mutex_unlock(&t->waiter->lock);
		pthread_exit(NULL);
	}
	print_line(t->input_data->start_time, t->philo->id, "is thinking");
	pthread_mutex_unlock(&t->waiter->lock);
}

void	*philosopher_thread(void *args)
{
	t_thread_data	*t;

	t = (t_thread_data *)args;
	while (TRUE)
	{
		if (t->waiter->someone_has_died)
		{
			pthread_exit(NULL);
		}
		else if (t->waiter->number_of_full_philosophers >= t->input_data->number_of_philosophers)
		{
			pthread_exit(NULL);
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
		usleep(100);
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
		
		// if (i == 0)
		// {
		// 	main_data.philosophers_data[i].p_r = &main_data.philosophers_data[i + 1];
		// 	main_data.philosophers_data[i].p_l = &main_data.philosophers_data[main_data.input_data.number_of_philosophers - 1];
		// }
		// else if (i == main_data.input_data.number_of_philosophers - 1)
		// {
		// 	main_data.philosophers_data[i].p_r = &main_data.philosophers_data[0];
		// 	main_data.philosophers_data[i].p_l = &main_data.philosophers_data[i - 1];
		// }
		// else
		// {
		// 	main_data.philosophers_data[i].p_r = &main_data.philosophers_data[i + 1];
		// 	main_data.philosophers_data[i].p_l = &main_data.philosophers_data[i - 1];
		// }
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
