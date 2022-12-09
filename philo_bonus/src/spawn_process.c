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

#include "philosophers_bonus.h"

void	*death_checker(void *arg)
{
	t_death_checker_thread_data	*d;

	d = (t_death_checker_thread_data *)arg;
	while (TRUE)
	{
		if (philo_time_is_greater_than_zero(\
			philo_time_since(d->dat->time_to_die)))
		{
			sem_wait(d->arg->sem->first_to_die);
			print_line(d->arg->input->start_time, d->arg->id, "died");
			sem_post(d->arg->sem->stop);
			usleep(1000000);
		}
	}
}

static void	set_starting_philo_values(t_philosopher_arg_data *arg, \
t_philosopher_own_data *dat)
{
	dat->times_to_eat = 0;
	if (arg->input->infinite_simulation == FALSE)
	{
		dat->times_to_eat = \
			arg->input->number_of_times_each_philosopher_must_eat;
	}
	dat->time_to_die = philo_time_add(arg->input->start_time, \
		arg->input->time_to_die);
}

void	run_philo(t_philosopher_arg_data arg)
{
	t_philosopher_own_data		dat;
	pthread_t					thread;
	t_death_checker_thread_data	thread_data;
	int							ret;

	set_starting_philo_values(&arg, &dat);
	thread_data.arg = &arg;
	thread_data.dat = &dat;
	ret = pthread_create(&thread, NULL, death_checker, &thread_data);
	if (ret)
	{
		printf("ERROR: Failed to create death checking thread\n");
		sem_post(arg.sem->stop);
	}
	if (dat.times_to_eat > 0 || arg.input->infinite_simulation == TRUE)
	{
		philo_eat(arg, &dat);
	}
	while (dat.times_to_eat > 0 || arg.input->infinite_simulation == TRUE)
	{
		philo_sleep(arg);
		philo_think(arg);
		philo_eat(arg, &dat);
	}
}

void	spawn_philo_processes(t_main_data *m)
{
	int						i;
	int						pid;
	t_philosopher_arg_data	arg;

	i = 1;
	sem_wait(m->sem.mutex);
	while (i <= m->input.number_of_philosophers)
	{
		pid = fork();
		if (pid)
		{
			m->philo_pid[i] = pid;
		}
		else
		{
			arg.id = i;
			arg.input = &m->input;
			arg.sem = &m->sem;
			run_philo(arg);
			exit(0);
		}
		i++;
	}
	sem_post(m->sem.mutex);
}
