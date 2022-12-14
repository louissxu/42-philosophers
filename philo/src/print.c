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

#include "philosophers.h"

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	printf("%5ld %i %s\n", time_since(start_time) / 1000, philo_name, str);
}

void	print_took_fork(struct timeval start_time, int philo_name, int fork_num)
{
	printf("%5ld Philosopher %i has taken fork number %i\n", \
		time_since(start_time), philo_name, fork_num);
}
