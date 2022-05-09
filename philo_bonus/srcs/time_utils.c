/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 19:45:02 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/05 23:31:27 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

time_t	get_mlsec_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	precise_sleep(uint64_t ms)
{
	time_t	wake_up_time;

	wake_up_time = get_mlsec_time() + ms;
	while (get_mlsec_time() < wake_up_time)
		usleep(100);
}

int	philo_precise_sleep(t_philosopher *philo, time_t ms)
{
	time_t	wake_up_time;

	wake_up_time = get_mlsec_time() + ms;
	while (get_mlsec_time() < wake_up_time)
	{
		if (is_someone_died(philo->mutex_simulation_stop))
			break ;
		usleep(100);
	}
	return (0);
}
