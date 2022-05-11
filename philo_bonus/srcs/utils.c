/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 11:23:50 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/11 18:13:13 by plouvel          ###   ########.fr       */
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
		usleep(100);
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

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void	display_status(t_philosopher *philo, char *str)
{
	time_t	curr_time;

	sem_wait(philo->sem_msg_print);
	curr_time = get_mlsec_time();
	printf(STR_P, curr_time - philo->start_time, philo->id, str);
	sem_post(philo->sem_msg_print);
}
