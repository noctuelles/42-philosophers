/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:25:26 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/05 16:26:13 by plouvel          ###   ########.fr       */
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

void	*supervisor_routine(void *arg)
{
	t_program		*program;
	t_philosopher	*philo;
	size_t		i;

	program = (t_program *) arg;
	while (get_mlsec_time() < program->start_time)
		;
	while (true)
	{
		i = 0;
		while (i < program->nbr_philo)
		{
			bool test = false;
			philo = &program->philos[i];
			pthread_mutex_lock(philo->mutex_time_of_death.addr);
			if (get_mlsec_time() >= (time_t) philo->mutex_time_of_death.data)
			{
				set_mutex(philo->mutex_simulation_stop, 1);
				pthread_mutex_lock(philo->mutex_msg->addr);
				printf(STR_P, get_mlsec_time() - philo->start_time, philo->id,
						STR_P_DIED);
				pthread_mutex_unlock(philo->mutex_msg->addr);
				test = true;
			}
			if (philo->meal_max != 0)
			{
				if (philo->meal_ate == philo->meal_max)
				{
					set_mutex(philo->mutex_simulation_stop, 1);
					test = true;
				}
			}
			pthread_mutex_unlock(philo->mutex_time_of_death.addr);
			if (test)
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
