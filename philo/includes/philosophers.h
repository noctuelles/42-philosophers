/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:38:20 by plouvel           #+#    #+#             */
/*   Updated: 2022/03/25 15:46:49 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define STR_P_TAKE_FORK "%lu %u has taken a fork\n"
# define STR_P_EATING    "%lu %u is eating\n"
# define STR_P_SLEEPING  "%lu %u is sleeping\n"
# define STR_P_THINKING  "%lu %u is thinking\n"
# define STR_P_DIED      "%lu %u died\n"

typedef struct s_program
{
	unsigned int nbr_philo;
	unsigned int nbr_philo_must_eat;
	unsigned int time_to_die;
	unsigned int time_to_eat;
	unsigned int time_to_sleep;
}				t_program;

#endif
