/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:25:26 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 23:49:03 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

bool	is_someone_died(t_mutex *mutex_simulation_stop)
{
	bool	ret;

	ret = false;
	pthread_mutex_lock(mutex_simulation_stop->addr);
	if (mutex_simulation_stop->data == 1)
		ret = true;
	pthread_mutex_unlock(mutex_simulation_stop->addr);
	return (ret);
}

static bool	is_philo_dying(t_philosopher *philo)
{
	time_t	curr_time;

	curr_time = get_mlsec_time();
	if (curr_time > philo->time_of_death)
	{
		set_mutex(philo->mutex_simulation_stop, 1);
		pthread_mutex_lock(philo->mutex_msg->addr);
		printf(STR_P_DEAD, curr_time - philo->start_time, philo->id,
			STR_P_DIED);
		pthread_mutex_unlock(philo->mutex_msg->addr);
		pthread_mutex_unlock(philo->mutex_eating.addr);
		return (true);
	}
	return (false);
}

void	ready_set_go(time_t start_time)
{
	while (get_mlsec_time() < start_time)
		;
}

void	*supervisor_routine(void *arg)
{
	t_program		*program;
	t_philosopher	*philo;
	bool			everyone_ate_enough;
	size_t			i;

	program = (t_program *) arg;
	ready_set_go(program->start_time);
	while (true)
	{
		i = 0;
		everyone_ate_enough = true;
		while (i < program->nbr_philo)
		{
			philo = &program->philos[i++];
			pthread_mutex_lock(philo->mutex_eating.addr);
			if (is_philo_dying(philo))
				return (NULL);
			if (philo->meal_ate < philo->meal_max)
				everyone_ate_enough = false;
			pthread_mutex_unlock(philo->mutex_eating.addr);
		}
		if (everyone_ate_enough && philo->meal_max != 0)
			return (set_mutex(philo->mutex_simulation_stop, 1));
	}
	return (NULL);
}
