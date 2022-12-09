/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:00:14 by lxu               #+#    #+#             */
/*   Updated: 2022/11/22 15:00:17 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H

# define BOOL int
# define TRUE 1
# define FALSE 0

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

# define STARVING_BUFFER 1000

/**
 * @var	t_input_data::time_to_die
*/
typedef struct s_input_data
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	BOOL			infinite_simulation;
	int				number_of_times_each_philosopher_must_eat;
	struct timeval	start_time;
}	t_input_data;

typedef struct s_waiter_data
{
	pthread_mutex_t	lock;
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
typedef struct s_philosopher_data
{
	int				id;
	int				fork_l;
	int				fork_r;
	int				times_eaten;
	int				state;
	struct timeval	last_ate;
}	t_philosopher_data;

typedef struct s_thread_data
{
	t_philosopher_data	*philo;
	t_philosopher_data	*philo_l;
	t_philosopher_data	*philo_r;
	t_input_data		*input_data;
	t_waiter_data		*waiter;
}	t_thread_data;

typedef struct s_main_data
{
	t_input_data		input_data;
	t_waiter_data		waiter;
	pthread_t			*threads;
	t_thread_data		*thread_data;
	t_philosopher_data	*philosophers;
}	t_main_data;

long int	time_delta(struct timeval initial, struct timeval final);
long int	time_since(struct timeval t);

void		print_line(struct timeval start_time, int philo_name, char *str);
void		print_took_fork(struct timeval start_time, int philo_name, \
				int fork_num);

void		philo_eating(void *args);
void		philo_sleeping(void *args);
void		philo_thinking(void *args);

int			philo_simple_atoi(const char *str);
BOOL		parse_args(int argc, char **argv, t_main_data *m);
BOOL		setup_mallocs(t_main_data *m);
void		setup_waiter(t_main_data *m);
void		setup_philosophers(t_main_data *m);
void		setup_thread_data(t_main_data *m);
BOOL		setup_threads(t_main_data *m);

void		free_all(t_main_data *m);

void		*philosopher_thread(void *args);

#endif
