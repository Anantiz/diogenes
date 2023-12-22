/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:49:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/22 18:12:32 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>

# include <string.h>
# include <stdlib.h>
# include <semaphore.h>
# include <pthread.h>
# include <signal.h>
# include <errno.h>

# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define STARVED -8798
# define DEATH_VAL 666
# define SUB_PROCESS_SUCCESS 0
# define SUB_PROCESS_FAILURE -1
# define SEM_FORK "sem_fork"
# define SEM_WAIT "sem_wait"
# define SEM_PRINT "sem_print"

/* Read-only once initialized */
typedef struct s_sim_data
{
	int				philo_count;
	unsigned int	meal_max;
	suseconds_t		start_time;
	suseconds_t		time_to_die;
	suseconds_t		time_to_eat;
	suseconds_t		time_to_sleep;
}t_sim_data;

typedef struct s_shared
{
	pid_t		*wait;
	sem_t		*forks_count;
	pid_t		*pid_list;
	t_sim_data	sim_data;
}t_shared;

typedef struct s_philosophers
{
	int				number;
	unsigned int	meal_count;
	suseconds_t		last_meal;
	sem_t			*forks_count;
	sem_t			*print_lock;
	t_shared		shared;
}t_philo;

typedef enum e_philo_state
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
}t_philo_state;

/* Core */
int			init_shared_resources(t_shared *shared, int argc, char **argv);
void		clean_shared(t_shared *shared);
int			spawn_philosophers(t_shared *shared);
// Routine
void		philosopher_routine(void *this_);
void		think(t_philo *this);
int			eat_then_sleep(t_philo *this);
int			get_forks(t_philo *this);

/* Philo Utils */

void		change_state(t_philo *this, t_philo_state state);
int			did_i_starve(t_philo *this);
int			fork_unlocker(t_philo *this);
void		do_one_philo(t_philo *this);
int			ft_usleep(t_philo *this, suseconds_t t);

/* GLOBAL UTILS*/

suseconds_t	get_time(void);
int			ft_atoi(const char *nptr);
int			ft_abs(int n);
void		*ft_calloc(size_t nmemb, size_t size);

#endif