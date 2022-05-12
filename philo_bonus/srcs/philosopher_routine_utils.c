/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 18:42:18 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 14:19:41 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static void	alone_philo_routine(t_philosopher *philo)
{
	get_in_sync(philo->start_time);
	printf(STR_P, get_mlsec_time() - philo->start_time, philo->id,
		STR_P_FORK);
	precise_sleep(philo->time_to_die);
	printf(STR_P_DEAD, get_mlsec_time() - philo->start_time, philo->id,
		STR_P_DIED);
	exit(PHILO_IS_DEAD);
}

void	philo_init_ipc(t_philosopher *philo)
{
	if (philo->nbr_philo == 1)
		alone_philo_routine(philo);
	sem_unlink(philo->sem_eat_name);
	if (pthread_create(&philo->supervisor_thread, NULL, supervisor_routine,
		philo) == -1)
		exit(cleanup(philo, PHILO_PTHREAD_ERR));
	pthread_detach(philo->supervisor_thread);
	philo->sem_forks = sem_open(STR_SEM_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->nbr_philo);
	if (philo->sem_forks == SEM_FAILED)
		exit(cleanup(philo, PHILO_SEM_ERR));
	philo->sem_msg_print = sem_open(STR_SEM_MSG_PRINT, O_CREAT,
			S_IRUSR | S_IWUSR, 1); 
	if (philo->sem_msg_print == SEM_FAILED)
		exit(cleanup(philo, PHILO_SEM_ERR));
	philo->sem_eat = sem_open(philo->sem_eat_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_eat == SEM_FAILED)
		exit(cleanup(philo, PHILO_SEM_ERR));
	sem_unlink(philo->sem_eat_name);
}
