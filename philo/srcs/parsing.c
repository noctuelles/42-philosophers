/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 23:09:39 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/05 23:12:03 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <limits.h>

static unsigned int	*ft_ato_ui(const char *nptr, unsigned int *val)
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
	if (program->nbr_philo > PHILO_HARD_LIMIT)
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
