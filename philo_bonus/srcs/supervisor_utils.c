/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 15:17:08 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 16:05:14 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/wait.h>

bool	can_continue_simulation(t_program *program)
{
	size_t	i;
	bool	continue_simulation;

	i = 0;
	continue_simulation = false;
	while (i < program->nbr_philo)
	{
		if (program->processes[i] != 0)
			continue_simulation = true;
		i++;
	}
	return (continue_simulation);
}

int	kill_them_all(t_program *program, int ret_code)
{
	size_t	i;

	i = 0;
	while (i < program->nbr_philo)
		kill(program->processes[i++], SIGTERM);
	return (ret_code);
}
