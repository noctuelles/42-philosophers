/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 14:24:58 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 16:21:24 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static void	*quit_launch(t_program *program, int err, size_t n)
{
	size_t	i;

	i = 0;
	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	else if (err == E_THREAD)
		ft_putstr_fd(STR_PTHREAD_C, STDERR_FILENO);
	else if (err == E_FORK)
		ft_putstr_fd(STR_FORK_ERR, STDERR_FILENO);
	i = 0;
	while (i < n)
		kill(program->processes[i], SIGKILL);
	return (NULL);
}

static inline void	philo_take_fork(t_philosopher *philo)
{
	sem_wait(philo->sem_forks);
	display_status(philo, STR_P_FORK);
	sem_wait(philo->sem_eat);
	philo->nbr_fork_holding += 1;
	sem_post(philo->sem_eat);
}

static inline void	philo_life_cycle(t_philosopher *philo)
{
	display_status(philo, STR_P_EATING);
	precise_sleep(philo->time_to_eat);
	sem_wait(philo->sem_eat);
	philo->time_of_death = get_mlsec_time() + philo->time_to_die;
	sem_post(philo->sem_eat);
	display_status(philo, STR_P_SLEEPING);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	sem_wait(philo->sem_eat);
	philo->nbr_fork_holding -= 2;
	philo->meal_ate += 1;
	sem_post(philo->sem_eat);
	precise_sleep(philo->time_to_sleep);
}

static void	philo_main(t_program *program)
{
	t_philosopher	*philo;

	philo = program->curr_philo;
	philo_init_ipc(program, philo);
	get_in_sync(philo->start_time);
	if (philo->id % 2 == 0)
		precise_sleep(philo->time_to_eat);
	while (true)
	{
		philo_take_fork(philo);
		philo_take_fork(philo);
		philo_life_cycle(philo);
		display_status(philo, STR_P_THINKING);
	}
}

t_philosopher	*launch_philos(t_program *program)
{
	size_t	i;
	pid_t	pid;

	i = 0;
	while (i < program->nbr_philo)
	{
		pid = fork();
		if (pid == -1)
			return (quit_launch(program, E_FORK, i));
		else if (pid)
		{
			program->processes[i] = pid;
			program->philos[i].pid = pid;
		}
		else if (pid == 0)
		{
			program->curr_philo = &program->philos[i];
			philo_main(program);
		}
		i++;
	}
	if (philo_watchdog(program) == -1)
		return (NULL);
	return (program->philos);
}
