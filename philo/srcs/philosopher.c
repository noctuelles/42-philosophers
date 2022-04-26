/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 15:22:09 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/26 18:53:40 by plouvel          ###   ########.fr       */
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
	if (philo->id % 2 == 0)
		ft_sleep_t(50);
	while (1)
	{
		pthread_mutex_lock(philo->fork[0].addr);
		display_status(philo, STR_P_FORK);
		pthread_mutex_lock(philo->fork[1].addr);
		display_status(philo, STR_P_FORK);

		display_status(philo, STR_P_EATING);
		ft_sleep_t(philo->time_to_eat);

		pthread_mutex_unlock(philo->fork[0].addr);
		pthread_mutex_unlock(philo->fork[1].addr);

		display_status(philo, STR_P_SLEEPING);
		ft_sleep_t(philo->time_to_sleep);

		display_status(philo, STR_P_THINKING);
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

t_philosopher	*launch_philos(t_program *program)
{
	size_t	i;

	i = 0;
	program->start_time = get_mlsec_time();
	while (i < program->nbr_philo)
	{
		if (pthread_create(&program->philos[i].thread, NULL, &philo_thread,
				&program->philos[i]) != 0)
			return (quit(E_THREAD));
		i++;
	}
	i = 0;
	while (i < program->nbr_philo)
	{
		pthread_join(program->philos[i].thread, NULL);
		i++;
	}
	return (program->philos);
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
		philos[i].msg_mutex = &program->msg_mutex;
		philos[i].start_time = &program->start_time;
		philos[i].philo_died = &program->philo_died;
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
		/*if ((i + 1) % 2 != 0)
		{
			t_mutex swap;

			swap = philos[i].fork[0];
			philos[i].fork[0] = philos[i].fork[1];
			philos[i].fork[1] = swap;
		}*/
		i++;
	}
	return (philos);
}
