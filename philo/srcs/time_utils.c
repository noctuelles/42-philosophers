/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 19:45:02 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/26 18:52:58 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

uint64_t	get_mlsec_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

unsigned int	ft_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	ft_sleep_t(size_t ms)
{
	unsigned int	curr;
	unsigned int	end;

	curr = ft_time();
	end = curr + ms;
	while (ft_time() < end)
		usleep(100);
}

void	display_status(t_philosopher *philo, char *str)
{
	pthread_mutex_lock(philo->msg_mutex->addr);
	printf(STR_P, get_mlsec_time() - *(philo->start_time), philo->id, str);
	pthread_mutex_unlock(philo->msg_mutex->addr);
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
