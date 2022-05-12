/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 11:23:50 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 19:03:02 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "philosophers.h"
#include <stdlib.h>

bool	is_digit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

void	*set_mutex(t_mutex *mutex, uint64_t value)
{
	pthread_mutex_lock(mutex->addr);
	mutex->data = value;
	pthread_mutex_unlock(mutex->addr);
	return (NULL);
}

void	display_status(t_philosopher *philo, char *str)
{
	pthread_mutex_lock(philo->mutex_msg->addr);
	pthread_mutex_lock (philo->mutex_simulation_stop->addr);
	if (philo->mutex_simulation_stop->data != 1)
		printf(STR_P, get_mlsec_time() - philo->start_time, philo->id, str);
	pthread_mutex_unlock (philo->mutex_simulation_stop->addr);
	pthread_mutex_unlock(philo->mutex_msg->addr);
}
