/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 23:27:03 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/11 18:42:58 by plouvel          ###   ########.fr       */
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
		kill(program->processes[i], SIGTERM);
	return (NULL);
}

void	kill_them_all(t_program *program)
{
	size_t	i;

	i = 0;
	while (i < program->nbr_philo)
		kill(program->processes[i++], SIGTERM);
}

void	philo_watchdog(t_program *program)
{
	size_t	i;
	int		proc_ret;
	int		ret_code;

	get_in_sync(program->start_time);
	while (1)
	{
		i = 0;
		while (i < program->nbr_philo)
		{
			if (program->processes[i] &&
					waitpid(program->processes[i], &proc_ret, WNOHANG) != 0)
			{
				if (WIFEXITED(proc_ret))
				{
					ret_code = WEXITSTATUS(proc_ret);
					if (ret_code == PHILO_IS_DEAD)
						return (kill_them_all(program));
					if (ret_code == PHILO_ATE_ENOUGH)
						program->processes[i] = 0;
				}
			}
			i++;
		}
		usleep(100);
	}
}

static inline void	philo_take_fork(t_philosopher *philo)
{
	sem_wait(philo->sem_forks);
	display_status(philo, STR_P_FORK);
}

static inline void	philo_life_cycle(t_philosopher *philo)
{
	display_status(philo, STR_P_EATING);
	precise_sleep(philo->time_to_eat);
	philo->time_of_death = get_mlsec_time() + philo->time_to_die;
	display_status(philo, STR_P_SLEEPING);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	precise_sleep(philo->time_to_sleep);
}

void	philo_main(t_philosopher *philo)
{
	philo_init_ipc(philo);
	/* Process syncronisation */

	get_in_sync(philo->start_time);

	/* Philo routine */
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
			philo_main(&program->philos[i]);
		i++;
	}
	philo_watchdog(program);
	return (program->philos);
}
