/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 15:22:09 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/23 15:59:06 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	*philo_thread(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) arg;
	philo->id = 0;
	return (NULL);
}

static void	*quit(int err)
{
	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	else if (err == E_THREAD)
		ft_putstr_fd(STR_PTHREAD_C, STDERR_FILENO);
	return (NULL);
}

t_philosopher	*launch_philos(t_philosopher *philos, unsigned int nbr_philo)
{
	size_t	i;

	i = 0;
	while (i < nbr_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_thread,
				&philos[i]) != 0)
			return (quit(E_THREAD));
		i++;
	}
	return (philos);
}

t_philosopher	*create_philos(t_program *program)
{
	size_t			i;
	t_philosopher	*philos;

	philos = (t_philosopher *) malloc(program->nbr_philo
			* sizeof(t_philosopher));
	if (!philos)
		return (quit(E_MALLOC));
	i = 0;
	while (i < program->nbr_philo)
	{
		memset(&philos[i], NUL, sizeof(t_philosopher));
		philos[i].id = i + 1;
		philos[i].time_to_die = program->time_to_die;
		philos[i].time_to_eat = program->time_to_eat;
		philos[i].time_to_sleep = program->time_to_sleep;
		philos[i].fork[0] = program->forks[i];
		if (program->nbr_philo != 1)
			philos[i].fork[1] = program->forks[i + 1];
		philos[i].status = P_THINKING;
		i++;
	}
	return (philos);
}
