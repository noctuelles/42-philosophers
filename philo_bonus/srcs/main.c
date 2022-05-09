/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 11:14:34 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/09 22:01:38 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

static void	*quit(t_err err)
{
	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	else if (err == E_SEM)
		ft_putstr_fd(STR_SEM_ERR, STDERR_FILENO);
	return (NULL);
}

int	setup_program(t_program *program, int argc, char **argv)
{
	size_t	arg;

	memset(program, 0, sizeof(t_program));
	if (argv[0])
		program->name = argv[0];
	else
		program->name = STR_DF_NAME;
	if (argc < 5 || argc > 6)
		return (printf(STR_USAGE, program->name));
	program->name = argv[0];
	arg = check_arguments(argc, argv);
	if (arg != 0)
		return (printf(STR_NOT_DIGIT, (unsigned int) arg));
	arg = parse_arguments(program, argc, argv);
	if (arg != 0)
	{
		if (arg == 1 && program->nbr_philo > PHILO_HARD_LIMIT)
			return (printf(STR_PHIL_OVERF, (unsigned int) arg));
		else
			return (printf(STR_VAL_OVERF, (unsigned int) arg, INT_MAX));
	}
	return (0);
}

void	*create_global_semaphore(t_program *program)
{
	program->forks = sem_open(STR_SEM_FORKS, O_CREAT | O_EXCL, 0644,
		program->nbr_philo); 
	if (program->forks == SEM_FAILED)
		return (quit(E_SEM));
	program->msg_print = sem_open(STR_SEM_MSG_PRINT, O_CREAT | O_EXCL, 0644,
		1);
	if (program->msg_print == SEM_FAILED)
	{
		sem_close(program->forks);
		sem_unlink(STR_SEM_FORKS);
		return (quit(E_SEM));
	}
	sem_unlink(STR_SEM_MSG_PRINT);
}

int	free_program(t_program *program, int ret)
{
	if (program->mutex_simulation_stop.addr)
	{
		pthread_mutex_destroy(program->mutex_simulation_stop.addr);
		free(program->mutex_simulation_stop.addr);
	}
	if (program->mutex_msg.addr)
	{
		pthread_mutex_destroy(program->mutex_msg.addr);
		free(program->mutex_msg.addr);
	}
	destroy_forks(program->mutex_forks, program->nbr_philo);
	destroy_philos(program->philos, program->nbr_philo);
	return (ret);
}

int	main(int argc, char **argv)
{
	t_program	program;

	if (setup_program(&program, argc, argv) != 0)
		return (1);
	program.mutex_forks = create_forks(program.nbr_philo);
	if (!program.mutex_forks)
		return (1);
	program.philos = create_philos(&program);
	if (!program.philos)
		return (1);
	if (!create_global_mutex(&program))
		return (free_program(&program, 1));
	if (!launch_philos(&program))
		return (free_program(&program, 1));
	return (free_program(&program, 0));
}
