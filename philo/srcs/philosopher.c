/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 15:22:09 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/05 16:17:49 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	philo_eat(t_philosopher *philo)
{
	pthread_mutex_lock(philo->fork[0].addr);
	display_status(philo, STR_P_FORK);
	pthread_mutex_lock(philo->fork[1].addr);
	display_status(philo, STR_P_FORK);

	display_status(philo, STR_P_EATING); 
	philo_precise_sleep(philo, philo->time_to_eat);

	pthread_mutex_lock(philo->mutex_time_of_death.addr);
	philo->mutex_time_of_death.data = get_mlsec_time() + philo->time_to_die;
	philo->meal_ate += 1;
	pthread_mutex_unlock(philo->mutex_time_of_death.addr);

	pthread_mutex_unlock(philo->fork[0].addr);
	pthread_mutex_unlock(philo->fork[1].addr);
}

void	*philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) arg;
	while (get_mlsec_time() < philo->start_time)
		;
	if (philo->id % 2 == 0)
		precise_sleep(philo->time_to_eat);
	while (is_someone_died(philo->mutex_simulation_stop) == false)
	{
		philo_eat(philo);

		display_status(philo, STR_P_SLEEPING);
		philo_precise_sleep(philo, philo->time_to_sleep);
		display_status(philo, STR_P_THINKING);
	}
	return (NULL);
}

static void	*quit_launch(t_program *program, int err, size_t n)
{
	size_t	i;

	i = 0;
	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	else if (err == E_THREAD)
		ft_putstr_fd(STR_PTHREAD_C, STDERR_FILENO);
	set_mutex(&program->mutex_simulation_stop, 1);
	while (i < n) 
		pthread_detach(program->philos[i++].thread);
	return (NULL);
}

t_philosopher	*launch_philos(t_program *program)
{
	size_t	i;

	i = 0;
	while (i < program->nbr_philo)
	{
		if (pthread_create(&program->philos[i].thread, NULL, &philo_routine,
				&program->philos[i]) != 0)
			return (quit_launch(program, E_THREAD, i));
		i++;
	}
	if (pthread_create(&program->supervisor_thread, NULL, &supervisor_routine,
			program) != 0)
		return (quit_launch(program, E_THREAD, i));
	pthread_detach(program->supervisor_thread);
	i = 0;
	while (i < program->nbr_philo)
		pthread_join(program->philos[i++].thread, NULL);
	return (program->philos);
}

static void	set_philo_misc(t_program *program, t_philosopher *philo,
		size_t i)
{
	t_mutex swap;

	philo->mutex_msg = &program->mutex_msg;
	philo->mutex_simulation_stop = &program->mutex_simulation_stop;
	philo->start_time = program->start_time;
	philo->id = i + 1;
	philo->time_to_die = program->time_to_die;
	philo->time_to_eat = program->time_to_eat;
	philo->time_to_sleep = program->time_to_sleep;
	philo->meal_max = program->nbr_philo_must_eat;
	philo->meal_ate = 0;
	philo->fork[0] = program->mutex_forks[i];
	if (program->nbr_philo != 1)
	{
		if (i == program->nbr_philo - 1)
			philo->fork[1] = program->mutex_forks[0];
		else
			philo->fork[1] = program->mutex_forks[i + 1];
	}
	if ((i + 1) % 2 != 0)
	{
		swap = philo->fork[0];
		philo->fork[0] = philo->fork[1];
		philo->fork[1] = swap;
	}
}

static void	*quit_creation(t_philosopher *philos, size_t n, int err)
{
	size_t	i;

	i = 0;
	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	else if (err == E_MUTEX)
		ft_putstr_fd(STR_MUTEX_ERR, STDERR_FILENO);
	while (i < n)
	{
		pthread_mutex_destroy(philos[i].mutex_time_of_death.addr);
		free(philos[i].mutex_time_of_death.addr);
		i++;
	}
	free(philos);
	return (NULL);
}

t_philosopher	*create_philos(t_program *program)
{
	size_t			i;
	t_philosopher	*philos;

	philos = (t_philosopher *) malloc(program->nbr_philo
		* sizeof(t_philosopher));
	if (!philos)
		return (quit_creation(philos, E_MALLOC, 0));
	i = 0;
	program->start_time = get_mlsec_time() + program->nbr_philo;
	while (i < program->nbr_philo)
	{
		philos[i].mutex_time_of_death.addr = malloc(sizeof(pthread_mutex_t));
		if (philos[i].mutex_time_of_death.addr == NULL)
			return (quit_creation(philos, i, E_MALLOC));
		if (pthread_mutex_init(philos[i].mutex_time_of_death.addr, NULL) != 0)
			return (quit_creation(philos, i, E_MUTEX));
		philos[i].mutex_time_of_death.data = program->start_time +
			program->time_to_die; 
		set_philo_misc(program, &philos[i], i);
		i++;
	}
	return (philos);
}

void	destroy_philos(t_philosopher *philos, unsigned int nbr_philo)
{
	size_t	i;

	i = 0;
	while (i < nbr_philo)
	{
		pthread_mutex_destroy(philos[i].mutex_time_of_death.addr);
		free(philos[i].mutex_time_of_death.addr);
		i++;
	}
	free(philos);
}
