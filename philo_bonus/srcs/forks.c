/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 14:55:58 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/05 15:25:27 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <unistd.h>

static void	*quit(t_mutex *forks, size_t i, t_err err)
{
	size_t	j;

	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	else if (err == E_MUTEX)
		ft_putstr_fd(STR_MUTEX_ERR, STDERR_FILENO);
	if (forks)
	{
		j = 0;
		while (j < i)
			pthread_mutex_destroy(forks[j++].addr);
		free(forks);
	}
	return (NULL);
}

/* create_forks() returns an array of mutex.
 * If anything fails, the function print an error message and returns NULL. */

t_mutex	*create_forks(unsigned int nbr_philo)
{
	t_mutex			*forks;
	size_t			i;

	forks = (t_mutex *) malloc(nbr_philo * sizeof(t_mutex));
	if (!forks)
		return (quit(NULL, 0, E_MALLOC));
	i = 0;
	while (i < nbr_philo)
	{
		forks[i].addr = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
		if (!forks[i].addr)
			return (quit(forks, i, E_MALLOC));
		forks[i].data = 0;
		if (pthread_mutex_init(forks[i].addr, NULL) != 0)
			return (quit(forks, i, E_MUTEX));
		i++;
	}
	return (forks);
}

void	destroy_forks(t_mutex *forks, unsigned int nbr_philo)
{
	size_t	i;

	i = 0;
	while (i < nbr_philo)
	{
		pthread_mutex_destroy(forks[i].addr);
		free(forks[i].addr);
		i++;
	}
	free(forks);
}
