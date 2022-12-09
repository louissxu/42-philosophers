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
	return ((int)(result * post_mult));
}

BOOL	parse_args(int argc, char **argv, t_main_data *m)
{
	int	ret;

	if (argc < 5 || argc > 6)
	{
		printf ("Error: Number of args needs to be 4 or 5\n");
		return (TRUE);
	}
	m->input_data.number_of_philosophers = philo_simple_atoi(argv[1]);
	m->input_data.time_to_die = philo_simple_atoi(argv[2]) * 1000;
	m->input_data.time_to_eat = philo_simple_atoi(argv[3]) * 1000;
	m->input_data.time_to_sleep = philo_simple_atoi(argv[4]) * 1000;
	m->input_data.infinite_simulation = TRUE;
	if (argc == 6)
	{
		m->input_data.infinite_simulation = FALSE;
		m->input_data.number_of_times_each_philosopher_must_eat = \
			philo_simple_atoi(argv[5]);
	}
	ret = gettimeofday(&m->input_data.start_time, NULL);
	if (ret)
	{
		printf("Error: Failure of gettimeofday, return value: %d\n", ret);
		return (TRUE);
	}
	return (FALSE);
}
