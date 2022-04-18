/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:38:20 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/18 11:53:25 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdbool.h>

# define STR_PGR_NAME    "./philo"
# define STR_P_TAKE_FORK "%lu %u has taken a fork\n"
# define STR_P_EATING    "%lu %u is eating\n"
# define STR_P_SLEEPING  "%lu %u is sleeping\n"
# define STR_P_THINKING  "%lu %u is thinking\n"
# define STR_P_DIED      "%lu %u died\n"
# define STR_ERROR       "error: "
# define STR_BAD_ARG     "bad argument (%u): "
# define STR_NOT_DIGIT   STR_ERROR STR_BAD_ARG "not a valid positive unsigned integer.\n"
# define STR_VAL_OVERF   STR_ERROR STR_BAD_ARG "value overflowing, max : \
%u.\n"

# define STR_USAGE        "usage : %s \
<number_of_philosophers> (ms) <time_to_die> (ms) \
<time_to_eat> (ms) <time_to_sleep> (ms) \
[number_of_times_each_philosopher_must_eat]\n"

typedef struct s_program
{
	char		*name;
	unsigned int nbr_philo;
	unsigned int nbr_philo_must_eat;
	unsigned int time_to_die;
	unsigned int time_to_eat;
	unsigned int time_to_sleep;
}				t_program;

typedef struct s_philosopher
{

}				t_philosopher;

bool	is_digit(int c);

#endif
