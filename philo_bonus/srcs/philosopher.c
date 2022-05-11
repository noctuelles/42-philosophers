/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 15:22:09 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/11 18:02:48 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void	set_philo_misc(t_program *program, t_philosopher *philo,
		size_t i)
{
	philo->start_time = program->start_time;
	philo->id = i + 1;
	philo->time_to_die = program->time_to_die;
	philo->time_to_eat = program->time_to_eat;
	philo->time_to_sleep = program->time_to_sleep;
	philo->time_of_death = program->start_time + philo->time_to_die;
	philo->meal_max = program->nbr_philo_must_eat;
	philo->nbr_philo = program->nbr_philo;
}

static void	*quit_creation(t_philosopher *philos, size_t n, int err)
{
	size_t	i;

	i = 0;
	if (err == E_MALLOC)
		ft_putstr_fd(STR_MALLOC, STDERR_FILENO);
	while (i < n)
	{
		free((void *) philos[i].sem_eat_name);
		i++;
	}
	free(philos);
	return (NULL);
}

static const char	*get_philo_sem_name(unsigned int n)
{
	size_t	nbr_digit;
	char	*sem_name;
	size_t	i;

	nbr_digit = 0;
	i = n;
	while (i)
	{
		nbr_digit++;
		i /= 10;
	}
	sem_name = malloc((sizeof(STR_SEM_EAT) + nbr_digit) * sizeof(char));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	ft_strcat(sem_name, STR_SEM_EAT);
	i = sizeof(STR_SEM_EAT) - 2 + nbr_digit;
 	while (n % 10)
	{
		sem_name[i--] = (n % 10) + '0';
		n /= 10;
	}
	sem_name[sizeof(STR_SEM_EAT) - 1 + nbr_digit] = '\0';
	return ((const char *) sem_name);
}

t_philosopher	*create_philos(t_program *program)
{
	size_t			i;
	t_philosopher	*philos;

	philos = malloc(program->nbr_philo * sizeof(t_philosopher));
	if (!philos)
		return (quit_creation(philos, 0, E_MALLOC));
	i = 0;
	program->start_time = get_mlsec_time() + 100 + program->nbr_philo;
	while (i < program->nbr_philo)
	{
		memset(&philos[i], 0, sizeof(t_philosopher));
		philos[i].sem_eat_name = get_philo_sem_name(i + 1);
		if (philos[i].sem_eat_name == NULL)
			return (quit_creation(philos, i, E_MALLOC));
		set_philo_misc(program, &philos[i], i);
		i++;
	}
	return (philos);
}

void	destroy_philos(t_philosopher *philos, unsigned int nbr_philo)
{
	size_t	i;

	i = 0;
	while (i < nbr_philo)
	{
		free((void *) philos[i].sem_eat_name);
		i++;
	}
	free(philos);
}
