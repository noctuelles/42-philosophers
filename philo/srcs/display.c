/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 12:31:43 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/05 13:49:20 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	display_status(t_philosopher *philo, char *str)
{
	pthread_mutex_lock(philo->mutex_msg->addr);

	pthread_mutex_lock (philo->mutex_simulation_stop->addr);
	if (philo->mutex_simulation_stop->data != 1)
		printf(STR_P, get_mlsec_time() - philo->start_time, philo->id, str);
	pthread_mutex_unlock (philo->mutex_simulation_stop->addr);

	pthread_mutex_unlock(philo->mutex_msg->addr);
}
