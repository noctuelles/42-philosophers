/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:38:20 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/22 23:06:32 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>

/******************************************************************************
 *                                  Macros                                    *
 *****************************************************************************/


# define STR_P           "\x1b[96m%-20lu\x1b[0m \x1b[93;1m%5u\x1b[0m \
\x1b[1m%20s\x1b[0m\n"
# define STR_P_DEAD      "\x1b[96m%-20lu\x1b[0m \x1b[93;1m%5u\x1b[0m \
\x1b[1m%25s\x1b[0m\n"

# define STR_P_EATING    "is eating"
# define STR_P_SLEEPING  "is sleeping"
# define STR_P_DIED      "\x1b[91mdied"
# define STR_P_FORK      "has taken a fork"
# define STR_P_THINKING  "is thinking"
# define STR_NUL         ""

# define STR_ERROR       "error: "
# define STR_BAD_ARG     "bad argument (%u): "
# define STR_NOT_DIGIT   STR_ERROR STR_BAD_ARG "not a valid positive unsigned \
integer.\n"
# define STR_VAL_OVERF   STR_ERROR STR_BAD_ARG "value overflowing, max : \
%u.\n"

# define STR_USAGE        "usage : %s \
<number_of_philosophers> (ms) <time_to_die> (ms) \
<time_to_eat> (ms) <time_to_sleep> (ms) \
[number_of_times_each_philosopher_must_eat]\n"

/******************************************************************************
 *                              Typedef & Enum                                *
 *****************************************************************************/

typedef struct s_program
{
	char			*name;
	pthread_t		*threads;
	unsigned int	nbr_philo;
	unsigned int	nbr_philo_must_eat;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
}				t_program;

typedef enum e_philo_status
{
	TAKE_FORK,
	EATING,
	THINKING,
	SLEEPING,
	DEAD
}				t_philo_status;

typedef struct s_philosopher
{
	unsigned int	id;
	time_t			living_since;
	time_t			last_meal;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	t_philo_status	status;
}				t_philosopher;

/******************************************************************************
 *                            Functions Prototype                             *
 *****************************************************************************/

bool	is_digit(int c);

static inline void	display_status(t_philo_status status, time_t timestamp,
		unsigned int philo_id)
{
	char	*str;

	if (status == EATING)
		str = STR_P_EATING;
	else if (status == SLEEPING)
		str = STR_P_SLEEPING;
	else if (status == DEAD)
		str = STR_P_DIED;
	else if (status == THINKING)
		str = STR_P_THINKING;
	else if (status == TAKE_FORK)
		str = STR_P_FORK;
	else
		str = STR_NUL;
	if (status == DEAD)
		printf(STR_P_DEAD, timestamp, philo_id, str);
	else
		printf(STR_P, timestamp, philo_id, str);
}

#endif
