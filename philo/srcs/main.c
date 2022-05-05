/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 11:14:34 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/05 16:14:23 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdio.h>

unsigned int	*ft_ato_ui(const char *nptr, unsigned int *val)
{
	uint64_t	result;

	result = 0;
	while (*nptr != '\0')
	{
		result = result * 10 + (*nptr++ - '0');
		if (result > INT_MAX)
			return (NULL);
	}
	*val = result;
	return (val);
}

size_t	parse_arguments(t_program *program, int argc, char **argv)
{
	if (!ft_ato_ui(argv[1], &program->nbr_philo))
		return (1);
	if (!ft_ato_ui(argv[2], &program->time_to_die))
		return (2);
	if (!ft_ato_ui(argv[3], &program->time_to_eat))
		return (3);
	if (!ft_ato_ui(argv[4], &program->time_to_sleep))
		return (4);
	if (argc == 6)
	{
		if (!ft_ato_ui(argv[5], &program->nbr_philo_must_eat))
			return (5);
	}
	else
		program->nbr_philo_must_eat = 0;
	return (0);
}

size_t	check_arguments(int argc, char **argv)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (i < (size_t) argc)
	{
		j = 0;
		if (argv[i][0] == '0' && argv[i][1] == '\0')
			return (i);
		while (argv[i][j] != '\0')
		{
			if (!is_digit(argv[i][j]))
				return (i);
			j++;
		}
		i++;
	}
	return (0);
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
		return (printf(STR_VAL_OVERF, (unsigned int) arg, INT_MAX));
	return (0);
}

#include <unistd.h>

static void *quit(int err)
{
	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	else if (err == E_MUTEX)
		ft_putstr_fd(STR_MUTEX_ERR, STDERR_FILENO);
	return (NULL);
}

void	*create_global_mutex(t_program *program)
{
	program->mutex_msg.addr = malloc(sizeof(pthread_mutex_t));
	if (program->mutex_msg.addr == NULL)
		return (quit(E_MALLOC));
	if (pthread_mutex_init(program->mutex_msg.addr, NULL) != 0)
		return (quit(E_MUTEX));
	program->mutex_simulation_stop.addr = malloc(sizeof(pthread_mutex_t));
	if (program->mutex_simulation_stop.addr == NULL)
		return (quit(E_MALLOC));
	if (pthread_mutex_init(program->mutex_simulation_stop.addr, NULL) != 0)
		return (quit(E_MUTEX));
	program->mutex_simulation_stop.data = 0;
	return (program);
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
	printf("Must eat : %d\n", program.nbr_philo_must_eat);
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
