/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 11:23:50 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 16:09:59 by plouvel          ###   ########.fr       */
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

void	get_in_sync(time_t start_time)
{
	while (get_mlsec_time() < start_time)
		;
}

void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

void	display_status(t_philosopher *philo, char *str)
{
	time_t	curr_time;

	sem_wait(philo->sem_msg_print);
	curr_time = get_mlsec_time();
	printf(STR_P, curr_time - philo->start_time, philo->id, str);
	sem_post(philo->sem_msg_print);
}
