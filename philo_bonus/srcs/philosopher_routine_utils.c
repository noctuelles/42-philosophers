/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 18:42:18 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/11 18:42:46 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <fcntl.h>
#include <stdlib.h>

void	philo_init_ipc(t_philosopher *philo)
{
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
