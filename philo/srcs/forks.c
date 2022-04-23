/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 14:55:58 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/23 15:21:55 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <unistd.h>

static void	*quit(pthread_mutex_t *forks, size_t i)
{
	size_t	j;

	j = 0;
	if (forks)
	{
		ft_putstr_fd(STR_MUTEX_ERR, STDERR_FILENO);
		while (j < i)
			pthread_mutex_destroy(&forks[j++]);
		free(forks);
	}
	else
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	return (NULL);
}

/* create_forks() returns an array of mutex.
 * If anything fails, the function print an error message and returns NULL. */

pthread_mutex_t	*create_forks(unsigned int nbr_philo)
{
	pthread_mutex_t	*forks;
	size_t			i;

	forks = (pthread_mutex_t *) malloc(nbr_philo * sizeof(pthread_mutex_t));
	if (!forks)
		return (quit(NULL, 0));
	i = 0;
	while (i < nbr_philo)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (quit(forks, i));
		i++;
	}
	return (forks);
}
