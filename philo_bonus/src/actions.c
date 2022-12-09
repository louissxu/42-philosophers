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

void	philo_eat(t_philosopher_arg_data arg, t_philosopher_own_data *dat)
{
	sem_wait(arg.sem->mutex);
	sem_wait(arg.sem->chopsticks);
	print_line(arg.input->start_time, arg.id, "took a fork");
	sem_wait(arg.sem->chopsticks);
	print_line(arg.input->start_time, arg.id, "took a fork");
	print_line(arg.input->start_time, arg.id, "is eating");
	dat->time_to_die = philo_time_add(philo_time_now(), arg.input->time_to_die);
	if (arg.input->infinite_simulation == FALSE && dat->times_to_eat >= 0)
	{
		(dat->times_to_eat)--;
		if (dat->times_to_eat == 0)
		{
			sem_post(arg.sem->full);
		}
	}
	sem_post(arg.sem->mutex);
	ft_timeval_sleep(arg.input->time_to_eat);
	sem_post(arg.sem->chopsticks);
	sem_post(arg.sem->chopsticks);
}

void	philo_sleep(t_philosopher_arg_data arg)
{
	print_line(arg.input->start_time, arg.id, "is sleeping");
	ft_timeval_sleep(arg.input->time_to_sleep);
}

void	philo_think(t_philosopher_arg_data arg)
{
	print_line(arg.input->start_time, arg.id, "is thinking");
}
