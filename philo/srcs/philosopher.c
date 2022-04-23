/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 15:22:09 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/23 20:48:03 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	philo_take_fork(t_philosopher *philo)
{
}

int	take_forks(t_philosopher *philo)
{
	struct timeval	curr;

	pthread_mutex_lock(philo->fork[0].addr);
	pthread_mutex_lock(philo->fork[1].addr);
	gettimeofday(&curr, NULL);
	if (diff_mlsec(philo->last_meal, curr) >= philo->time_to_die)
	{
		display_status(philo, P_DEAD);
		return (-1);
	}
}

void	*philo_thread(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) arg;
	while (1)
	{
		if (take_forks(philo) == -1)
			return (NULL);
		display_status(philo, P_TAKE_FORK);
		display_status(philo, P_EATING);
		usleep(philo->time_to_eat * 1000);

		gettimeofday(&philo->last_meal, NULL);
		pthread_mutex_unlock(philo->fork[0].addr);
		pthread_mutex_unlock(philo->fork[1].addr);

		display_status(philo, P_SLEEPING);
		usleep(philo->time_to_sleep * 1e3);
		display_status(philo, P_THINKING);
	}
	// philo pense et tente donc de lock ses fourchettes

	//printf("Hi i'm philo %u created at %lu and %lu\n", philo->id, philo->creation.tv_sec, philo->creation.tv_usec);
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

t_philosopher	*launch_philos(t_mutex *forks, t_philosopher *philos,
		unsigned int nbr_philo)
{
	size_t	i;

	i = 0;
	lock_forks(forks, nbr_philo);
	while (i < nbr_philo)
	{
		gettimeofday(&philos[i].launch_time, NULL);
		if (pthread_create(&philos[i].thread, NULL, &philo_thread,
				&philos[i]) != 0)
			return (quit(E_THREAD));
		gettimeofday(&philos[i].last_meal, NULL);
		i++;
	}
	i = 0;
	unlock_forks(forks, nbr_philo);
	while (i < nbr_philo)
	{
		pthread_join(philos[i].thread, &philos[i].ret);
		size_t j = 0;
		while (j < nbr_philo)
		{
			if (j != i)
				pthread_detach(philos[j].thread);
			j++;
		}
		exit(0);
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
		{
			if (i == program->nbr_philo - 1)
				philos[i].fork[1] = program->forks[0];
			else
				philos[i].fork[1] = program->forks[i + 1];
		}
		i++;
	}
	return (philos);
}
