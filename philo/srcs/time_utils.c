/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 19:45:02 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/30 14:14:47 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
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
		if (is_all_philo_alive(philo) == false)
			return (-1);
		usleep(100);
	}
	return (0);
}

/*bool	can_sleep(struct timeval schedule_time)
{
	struct timeval curr_time;

	gettimeofday(&curr_time, NULL);
	if (curr_time.tv_sec >= schedule_time.tv_sec
		&& curr_time.tv_usec >= schedule_time.tv_usec)
		return (false);
	return (true);
}

void	smart_sleep(unsigned int mlsec)
{
	struct timeval	schedule_time;
	time_t			sec_to_add;
	suseconds_t		usec_to_add;

	sec_to_add = mlsec * 0.001;
	if (sec_to_add > 0)
		mlsec -= (sec_to_add * 1000);
	usec_to_add = mlsec * 1000;
	gettimeofday(&schedule_time, NULL);
	schedule_time.tv_sec += sec_to_add;
	schedule_time.tv_usec += usec_to_add;
	while (can_sleep(schedule_time))
		usleep(50);
}*/
