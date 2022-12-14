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

void	wait_for_all_processes_to_exit(t_main_data *m)
{
	int	i;
	int	ignore_ret;

	i = 1;
	while (i <= m->input.number_of_philosophers)
	{
		waitpid(m->philo_pid[i], &ignore_ret, 0);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_main_data	m;
	int			ret;

	ret = parse_input_args(argc, argv, &m);
	if (ret)
		return (EXIT_SUCCESS);
	ret = malloc_main_data(&m);
	if (ret)
		return (EXIT_SUCCESS);
	setup_semaphores(&m);
	ret = setup_state_checks(&m);
	if (ret)
	{
		free_mallocs(&m);
		return (EXIT_SUCCESS);
	}
	spawn_philo_processes(&m);
	wait_for_all_processes_to_exit(&m);
	clean_up_semaphores();
	free_mallocs(&m);
	return (0);
}
