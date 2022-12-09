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

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	printf("%5ld %i %s\n", philo_ms_since(start_time), philo_name, str);
}

void	print_incorrect_arg_count_error_message(void)
{
	printf("%s\n%s\n%s\n",
		"Incorrect number of arguments. Needs 4 or 5 arguments.", \
		"Usage: ./philo_bonus number_of_philosophers time_to_die time_to_eat", \
		"time_to_sleep [number_of_times_each_philosopher_must_eat]");
}

void	print_small_time_values_warn_message(void)
{
	printf("%s %s\n%s\n",
		"WARN: time_to_die, time_to_eat, and time_to_sleep", \
		"are expected to be >= 60ms.", \
		"Behaviour with values <60ms may be unreliable.");
}
