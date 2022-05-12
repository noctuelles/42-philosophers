/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:25:26 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 12:06:46 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int	cleanup(t_philosopher *philo, int ret_code)
{
	if (philo->sem_forks)
		sem_close(philo->sem_forks);
	if (philo->sem_eat)
		sem_close(philo->sem_eat);
	if (philo->sem_msg_print)
		sem_close(philo->sem_msg_print);
	if (ret_code == PHILO_SEM_ERR)
		ft_putstr_fd(STR_SEM_ERR, STDERR_FILENO);
	else if (ret_code == PHILO_PTHREAD_ERR)
		ft_putstr_fd(STR_PTHREAD_C, STDERR_FILENO);
	exit(ret_code);
}

static inline void	drop_forks_n_leave(t_philosopher *philo)
{
	unsigned int nbr_forks = philo->nbr_fork_holding;

	sem_post(philo->sem_eat);
	if (nbr_forks >= 1)
		sem_post(philo->sem_forks);
	if (nbr_forks == 2)
		sem_post(philo->sem_forks);
	exit(cleanup(philo, PHILO_ATE_ENOUGH));
}

void	*supervisor_routine(void *arg)
{
	t_philosopher	*philo;

	philo = arg;
	get_in_sync(philo->start_time);
	while (true)
	{
		sem_wait(philo->sem_eat);
		if (get_mlsec_time() >= philo->time_of_death)
		{
			sem_wait(philo->sem_msg_print);
			printf(STR_P_DEAD, get_mlsec_time() - philo->start_time, philo->id,
				STR_P_DIED);
			exit(cleanup(philo, PHILO_IS_DEAD));
		}
		if (philo->meal_max && philo->meal_ate >= philo->meal_max)
			drop_forks_n_leave(philo);
		sem_post(philo->sem_eat);
		usleep(8000);
	}
}
