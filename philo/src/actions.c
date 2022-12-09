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
	if (time_since(t->philo->last_ate) < t->input_data->time_to_eat + t->input_data->time_to_sleep)
	{
		return ;
	}
	print_line(t->input_data->start_time, t->philo->id, "is thinking");
	t->philo->state = 3;
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
	// If forks are in use
	if (t->waiter->fork_in_use[t->philo->fork_l] == TRUE || t->waiter->fork_in_use[t->philo->fork_r] == TRUE)
	{
		return ;
	}
	// If the forks are the same fork
	if (t->philo->fork_l == t->philo->fork_r)
	{
		return ;
	}
	// If either neighbour is hungrier and one cycle away from dying
	if ((time_since(t->philo_l->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_l->last_ate) + t->input_data->time_to_eat + STARVING_BUFFER > t->input_data->time_to_die) || \
		(time_since(t->philo_r->last_ate) > time_since(t->philo->last_ate) && \
		time_since(t->philo_r->last_ate) + t->input_data->time_to_eat + STARVING_BUFFER > t->input_data->time_to_die))
	{
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
}
