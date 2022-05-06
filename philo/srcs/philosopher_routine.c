/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 23:27:03 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/06 16:13:38 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

void	philo_eat(t_philosopher *philo)
{
	pthread_mutex_lock(philo->fork[0].addr);
	display_status(philo, STR_P_FORK);
	pthread_mutex_lock(philo->fork[1].addr);
	display_status(philo, STR_P_FORK);
	display_status(philo, STR_P_EATING);
	philo_precise_sleep(philo, philo->time_to_eat);
	pthread_mutex_lock(philo->mutex_eating.addr);
	philo->time_of_death = get_mlsec_time() + philo->time_to_die;
	philo->time_last_meal = get_mlsec_time();
	philo->meal_ate += 1;
	pthread_mutex_unlock(philo->mutex_eating.addr);
	pthread_mutex_unlock(philo->fork[0].addr);
	pthread_mutex_unlock(philo->fork[1].addr);
}

void	*philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) arg;
	ready_set_go(philo->start_time);
	if (philo->id % 2 == 0)
		philo_precise_sleep(philo, philo->time_to_eat);
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
