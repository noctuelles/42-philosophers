/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 11:14:34 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/27 12:38:38 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

unsigned long	*ft_ato_ui(const char *nptr, unsigned long *result)
{
	*result = 0;
	while (*nptr != '\0')
	{
		*result = *result * 10 + (*nptr++ - '0');
		if (*result > INT_MAX)
			return (NULL);
	}
	return (result);
}

size_t	parse_arguments(t_program *program, int argc, char **argv)
{
	if (!ft_ato_ui(argv[1], (unsigned long *) &program->nbr_philo))
		return (1);
	if (!ft_ato_ui(argv[2], (unsigned long *) &program->time_to_die))
		return (2);
	if (!ft_ato_ui(argv[3], (unsigned long *) &program->time_to_eat))
		return (3);
	if (!ft_ato_ui(argv[4], (unsigned long *) &program->time_to_sleep))
		return (4);
	if (argc == 6)
	{
		if (!ft_ato_ui(argv[5],
				(unsigned long *) &program->nbr_philo_must_eat))
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

int	create_global_mutex(t_program *program)
{
	program->msg_mutex.addr = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(program->msg_mutex.addr, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_program	program;

	if (setup_program(&program, argc, argv) != 0)
		return (1);
	program.forks = create_forks(program.nbr_philo);
	if (!program.forks)
		return (1);
	program.philos = create_philos(&program);
	if (!program.philos)
		return (1);
	create_global_mutex(&program);
	program.philo_died.data = 0;
	if (!launch_philos(&program))
		return (1);
	return (0);
}
