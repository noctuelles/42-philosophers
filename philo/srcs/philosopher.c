/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 15:22:09 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/03 16:09:28 by plouvel          ###   ########.fr       */
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
	while (get_mlsec_time() < philo->start_time)
		;
	display_status(philo, "youpi");
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
	lock_forks(program->mutex_forks, program->nbr_philo);
	while (i < program->nbr_philo)
	{
		if (pthread_create(&program->philos[i].thread, NULL, &philo_thread,
				&program->philos[i]) != 0)
			return (quit(E_THREAD));
		i++;
	}
	i = 0;
	if (pthread_create(&program->master_thread, NULL,
			&master_routine, program) != 0) 
		return (quit(E_THREAD));
	pthread_detach(program->master_thread);
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
	program->start_time = get_mlsec_time() + program->nbr_philo;
	while (i < program->nbr_philo)
	{
		memset(&philos[i], NUL, sizeof(t_philosopher));
		philos[i].mutex_msg = &program->mutex_msg;
		philos[i].mutex_simulation_stop = &program->mutex_simulation_stop;
		philos[i].start_time = program->start_time;
		philos[i].id = i + 1;
		philos[i].time_to_die = program->time_to_die;
		philos[i].time_to_eat = program->time_to_eat;
		philos[i].time_to_sleep = program->time_to_sleep;
		philos[i].fork[0] = program->mutex_forks[i];
		if (program->nbr_philo != 1)
		{
			if (i == program->nbr_philo - 1)
				philos[i].fork[1] = program->mutex_forks[0];
			else
				philos[i].fork[1] = program->mutex_forks[i + 1];
		}
		i++;
	}
	return (philos);
}
