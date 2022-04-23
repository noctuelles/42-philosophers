/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:38:20 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/23 20:51:00 by plouvel          ###   ########.fr       */
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

# define STR_P           "\x1b[96m%-10lu\x1b[0m \x1b[93;1m%5u\x1b[0m \
\x1b[1m%20s\x1b[0m\n"
# define STR_P_DEAD      "\x1b[96m%-10lu\x1b[0m \x1b[93;1m%5u\x1b[0m \
\x1b[1m%25s\x1b[0m\n"

# define STR_P_EATING    "is eating"
# define STR_P_SLEEPING  "is sleeping"
# define STR_P_DIED      "\x1b[91mdied"
# define STR_P_FORK      "has taken a fork"
# define STR_P_THINKING  "is thinking"
# define STR_NUL         ""
# define NUL             0

# define STR_ERROR       "error: "
# define STR_BAD_ARG     "bad argument (%u): "
# define STR_NOT_DIGIT   STR_ERROR STR_BAD_ARG "not a valid positive unsigned \
integer.\n"
# define STR_VAL_OVERF   STR_ERROR STR_BAD_ARG "value overflowing, max : \
%u.\n"
# define STR_MALLOC      "fatal: system cannot allocate memory.\n"
# define STR_MUTEX_ERR   "fatal: mutex: system cannot create mutex.\n"
# define STR_PTHREAD_C   "fatal: thread: insufficient ressources or\
 system-imposed limit on the number of threads.\n"

# define STR_USAGE        "usage: %s \
<number_of_philosophers> (ms) <time_to_die> (ms) \
<time_to_eat> (ms) <time_to_sleep> (ms) \
[number_of_times_each_philosopher_must_eat]\n"

/******************************************************************************
 *                              Typedef & Enum                                *
 *****************************************************************************/

typedef struct s_philosopher t_philosopher;

typedef struct	e_mutex
{
	pthread_mutex_t	*addr;
}				t_mutex;

typedef struct s_program
{
	char			*name;
	t_mutex			*forks;
	t_philosopher	*philos;
	unsigned int	nbr_philo;
	unsigned int	nbr_philo_must_eat;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
}				t_program;

typedef enum e_philo_status
{
	P_TAKE_FORK,
	P_EATING,
	P_THINKING,
	P_SLEEPING,
	P_DEAD
}				t_philo_status;

typedef enum e_err
{
	E_MALLOC = 0,
	E_THREAD = 1,
	E_MUTEX
}				t_err;

struct s_philosopher
{
	unsigned int	id;
	pthread_t		thread;
	t_mutex			fork[2];
	void			*ret;
	struct timeval	last_meal;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	struct timeval	launch_time;
	t_philo_status	status;
};

/******************************************************************************
 *                            Functions Prototype                             *
 *****************************************************************************/

/* utils.c */

bool			is_digit(int c);
void			ft_putstr_fd(const char *s, int fd);

/* forks.c */

t_mutex	*create_forks(unsigned int nbr_philo);

void	lock_forks(t_mutex *forks, unsigned int nbr_philo);
void	unlock_forks(t_mutex *forks, unsigned int nbr_philo);

void	printf_forks_addr(pthread_mutex_t *forks, unsigned int nbr_philo);

/* philosophers.c */

t_philosopher	*create_philos(t_program *program);
t_philosopher	*launch_philos(t_mutex *forks, t_philosopher *philos,
		unsigned int nbr_philo);

/* time_utils.c */

time_t	diff_mlsec(struct timeval t1, struct timeval t2);

/* inline function */

static inline void	display_status(t_philosopher *philo, t_philo_status status)
{
	char			*str;
	struct timeval	curr_time;

	if (status == P_EATING)
		str = STR_P_EATING;
	else if (status == P_SLEEPING)
		str = STR_P_SLEEPING;
	else if (status == P_DEAD)
		str = STR_P_DIED;
	else if (status == P_THINKING)
		str = STR_P_THINKING;
	else if (status == P_TAKE_FORK)
		str = STR_P_FORK;
	else
		str = STR_NUL;
	gettimeofday(&curr_time, NULL);
	if (status == P_DEAD)
		printf(STR_P_DEAD, diff_mlsec(philo->launch_time, curr_time), philo->id, str);
	else
		printf(STR_P, diff_mlsec(philo->launch_time, curr_time), philo->id, str);
}

#endif
