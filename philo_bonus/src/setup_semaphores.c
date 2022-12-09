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

void	clean_up_semaphores(void)
{
	sem_t	*s_ptr;
	int		i;
	char	*sem_names[5];
	char	*s_name;

	sem_names[0] = SEM_MUTEX;
	sem_names[1] = SEM_CHOPSTICKS;
	sem_names[2] = SEM_FULL;
	sem_names[3] = SEM_STOP;
	sem_names[4] = SEM_FIRST_TO_DIE;
	i = 0;
	while (i < 5)
	{
		s_name = sem_names[i];
		s_ptr = sem_open(s_name, 0);
		sem_close(s_ptr);
		sem_unlink(s_name);
		i++;
	}
}

void	setup_semaphores(t_main_data *m)
{
	clean_up_semaphores();
	m->sem.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0600, 1);
	m->sem.chopsticks = sem_open(SEM_CHOPSTICKS, O_CREAT | O_EXCL, 0600, \
		m->input.number_of_philosophers);
	m->sem.full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0600, 0);
	m->sem.stop = sem_open(SEM_STOP, O_CREAT | O_EXCL, 0600, 0);
	m->sem.first_to_die = sem_open(SEM_FIRST_TO_DIE, O_CREAT | O_EXCL, 0600, 1);
}
