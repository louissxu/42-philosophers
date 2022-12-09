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

long int	time_delta(struct timeval initial, struct timeval final)
{
	long int	delta_ms;

	delta_ms = (final.tv_sec - initial.tv_sec) * 1000000 + \
		(final.tv_usec - initial.tv_usec);
	return (delta_ms);
}

long int	time_since(struct timeval t)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (time_delta(t, current_time));
}
