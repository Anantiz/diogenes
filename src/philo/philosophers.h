/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:49:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/19 13:58:10 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdio.h>

# define STARVED -8798
# define MUTEX_LOCK_ERROR "Critical error while attempting to lock a mutex\n"
# define MLE_LEN 49
# define DEATH_VAL 666

/* Read-only after initialization */
typedef struct s_sim_data
{
	int				philo_count;
	unsigned int	meal_max;
	suseconds_t		start_time;
	suseconds_t		time_to_die;
	suseconds_t		time_to_eat;
	suseconds_t		time_to_sleep;
}t_sim_data;

/*
Forks are a mutex array:
	A locked mutex means a used fork
	An available mutex means an available fork
*/
typedef struct s_shared
{
	int				death;
	int				wait;
	t_sim_data		sim_data;
	int				*forks_state;
	pthread_mutex_t	*forks_lock;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*one_to_rule_them_all;
	pthread_t		*philosophers_id;
}t_shared;

/*
N + 1 = (N + 1) % philo_count

As an internal standard:
	left-fork = this.forks[this.number]
	right-fork = this.forks[(this.number + 1) % this.number]
*/
typedef struct s_philosophers
{
	int				number;
	unsigned int	meal_count;
	suseconds_t		last_meal;
	t_shared		*shared;
}t_philo;

typedef enum e_philo_state
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
}t_philo_state;

/* Core -----------------------------------------*/
int			init_shared_resources(t_shared *shared, int argc, char **argv);
void		clean_shared(t_shared *shared);
int			spawn_philosophers(t_shared *shared);
// Routine
void		*philosopher_routine(void *this_);
// Forks
int			get_forks(t_philo *this);
void		release_forks_state(t_philo *this);
int			release_forks_mutex(t_philo *this);
// States
int			eat_then_sleep(t_philo *this);
int			think(t_philo *this);

/* Philo Utils ----------------------------------*/

void		do_one_philo(t_philo *this);
int			did_i_starve(t_philo *this);
int			ft_usleep(t_philo *this, suseconds_t t);
int			change_state(t_philo *this, t_philo_state state);

/* GLOBAL UTILS ---------------------------------*/

suseconds_t	get_time(void);
int			ft_abs(int n);
int			ft_atoi(const char *nptr);
void		*ft_calloc(size_t nmemb, size_t size);

#endif