/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:25:26 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 16:05:49 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int	cleanup(t_program *program, t_philosopher *philo, int ret_code)
{
	sem_post(philo->sem_eat);
	if (ret_code == PHILO_SEM_ERR)
		ft_putstr_fd(STR_SEM_ERR, STDERR_FILENO);
	else if (ret_code == PHILO_PTHREAD_ERR)
		ft_putstr_fd(STR_PTHREAD_C, STDERR_FILENO);
	free(program->processes);
	exit(ret_code);
}

static inline void	drop_forks_n_leave(t_program *program,
		t_philosopher *philo)
{
	unsigned int	nbr_forks;

	nbr_forks = philo->nbr_fork_holding;
	sem_post(philo->sem_eat);
	if (nbr_forks >= 1)
		sem_post(philo->sem_forks);
	if (nbr_forks == 2)
		sem_post(philo->sem_forks);
	exit(cleanup(program, philo, PHILO_ATE_ENOUGH));
}

static inline int	check_philo_process(t_program *program, pid_t *pid)
{
	int		proc_ret;
	int		ret_code;

	if (*pid && waitpid(*pid, &proc_ret, WNOHANG) != 0)
	{
		if (WIFEXITED(proc_ret))
		{
			ret_code = WEXITSTATUS(proc_ret);
			if (ret_code == PHILO_IS_DEAD)
				return (kill_them_all(program, 1));
			if (ret_code == PHILO_ATE_ENOUGH)
				*pid = 0;
			if (ret_code == PHILO_PTHREAD_ERR
				|| ret_code == PHILO_SEM_ERR)
				return (kill_them_all(program, -1));
		}
	}
	return (0);
}

int	philo_watchdog(t_program *program)
{
	size_t	i;
	int		ret_code;

	get_in_sync(program->start_time);
	while (can_continue_simulation(program))
	{
		i = 0;
		while (i < program->nbr_philo)
		{
			ret_code = check_philo_process(program, &program->processes[i]);
			if (ret_code == 1 || ret_code == -1)
				return (ret_code);
			i++;
		}
	}
	return (0);
}

void	*supervisor_routine(void *arg)
{
	t_program		*program;
	t_philosopher	*philo;

	program = arg;
	philo = program->curr_philo;
	get_in_sync(philo->start_time);
	while (true)
	{
		sem_wait(philo->sem_eat);
		if (get_mlsec_time() >= philo->time_of_death)
		{
			sem_wait(philo->sem_msg_print);
			printf(STR_P_DEAD, get_mlsec_time() - philo->start_time, philo->id,
				STR_P_DIED);
			exit(cleanup(program, philo, PHILO_IS_DEAD));
		}
		if (philo->meal_max && philo->meal_ate >= philo->meal_max)
			drop_forks_n_leave(program, philo);
		sem_post(philo->sem_eat);
		usleep(6000);
	}
}
