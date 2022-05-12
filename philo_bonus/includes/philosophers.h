/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 15:38:20 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 16:22:48 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdint.h>

/******************************************************************************
 *                                  Macros                                    *
 *****************************************************************************/

# define STR_DF_NAME     "./philo"

# define STR_P           "\x1b[96m%-10lu\x1b[0m \x1b[93;1m%5u\x1b[0m \
\x1b[1m%20s\x1b[0m\n"
# define STR_P_DEAD      "\x1b[96m%-10lu\x1b[0m \x1b[93;1m%5u\x1b[0m \
\x1b[1m%25s\x1b[0m\n"

# define STR_P_EATING    "is eating"
# define STR_P_SLEEPING  "is sleeping"
# define STR_P_DIED      "\x1b[91mdied"
# define STR_P_FORK      "has taken a fork"
# define STR_P_THINKING  "is thinking"

# define STR_NOT_DIGIT   "error: bad argument (%u): not a valid positive \
unsigned integer.\n"
# define STR_VAL_OVERF   "error: bad argument (%u): value overflowing, max : \
%u.\n"
# define STR_PHIL_OVERF  "error: bad argument (%u): maximum number of \
philosophers reached.\n"
# define STR_MALLOC      "fatal: system cannot allocate memory.\n"
# define STR_SEM_ERR     "fatal: semaphore: system cannot create semaphore.\n\
you could try to empty smh folder using : 'rm /dev/shm/sem*'.\n"
# define STR_FORK_ERR    "fatal cannot create fork.\n"
# define STR_PTHREAD_C   "fatal: thread: insufficient ressources or\
 system-imposed limit on the number of threads.\n"

# define STR_USAGE        "usage: %s \
<number_of_philosophers> (ms) <time_to_die> (ms) \
<time_to_eat> (ms) <time_to_sleep> (ms) \
[number_of_times_each_philosopher_must_eat]\n"

# define PHILO_HARD_LIMIT 1000

# define STR_SEM_FORKS     "philo_global_forks"
# define STR_SEM_MSG_PRINT "philo_global_msg_print"
# define STR_SEM_EAT       "philo_local_eat_"

# define PHILO_PTHREAD_ERR 5
# define PHILO_SEM_ERR     10
# define PHILO_ATE_ENOUGH  21
# define PHILO_IS_DEAD     42

/******************************************************************************
 *                              Typedef & Enum                                *
 *****************************************************************************/

typedef struct s_philosopher	t_philosopher;

typedef struct s_program
{
	char			*name;
	t_philosopher	*curr_philo;
	t_philosopher	*philos;
	pid_t			*processes;
	sem_t			*sem_forks;
	sem_t			*sem_msg_print;
	unsigned int	nbr_philo;
	unsigned int	nbr_philo_must_eat;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	time_t			start_time;
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
	E_MALLOC,
	E_THREAD,
	E_FORK,
	E_SEM
}				t_err;

struct s_philosopher
{
	pid_t			pid;
	pthread_t		supervisor_thread;
	sem_t			*sem_forks;
	sem_t			*sem_msg_print;
	sem_t			*sem_eat;
	const char		*sem_eat_name;
	unsigned int	nbr_philo;
	unsigned int	nbr_fork_holding;
	unsigned int	id;
	unsigned int	meal_max;
	unsigned int	meal_ate;
	time_t			start_time;
	time_t			time_of_death;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
};

/******************************************************************************
 *                            Functions Prototype                             *
 *****************************************************************************/

/* utils.c */

bool			is_digit(int c);
void			get_in_sync(time_t start_time);
void			ft_putstr_fd(const char *s, int fd);

/* string.c */

char			*ft_strcat(char *dest, const char *src);
size_t			ft_strlen(const char *s);

/* philosophers.c */

t_philosopher	*create_philos(t_program *program);
void			destroy_philos(t_philosopher *philos, unsigned int nbr_philo);

/* philosophers_routine.c */

t_philosopher	*launch_philos(t_program *program);

/* philosophers_routine_utils.c */

void			philo_init_ipc(t_program *program, t_philosopher *philo);

/* display.c */

void			display_status(t_philosopher *philo, char *str);

/* time_utils.c */

time_t			get_mlsec_time(void);
void			precise_sleep(uint64_t ms);

/* supervisor.c */

int				philo_watchdog(t_program *program);
void			*supervisor_routine(void *arg);
int				cleanup(t_program *program, t_philosopher *philo,
					int ret_code);

/* supervisor_utils.c */

bool			can_continue_simulation(t_program *program);
int				kill_them_all(t_program *program, int ret_code);

/* parsing.c */

size_t			check_arguments(int argc, char **argv);
size_t			parse_arguments(t_program *program, int argc, char **argv);

#endif
