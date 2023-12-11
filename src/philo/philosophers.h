/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:49:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/11 15:52:21 by aurban           ###   ########.fr       */
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

# define STARVED -3
# define MUTEX_LOCK_ERROR "Critical error while attempting to lock a mutex\n"
# define MLE_LEN 49

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
typedef struct s_table
{
	int				death;
	int				wait;
	int				*forks_state;
	t_sim_data		sim_data;
	pthread_mutex_t	*forks_state_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	pthread_t		*philosophers_id;
}t_table_data;

/*
N + 1 = (N + 1) % philo_count

As an internal standard:
	left-fork = this.forks[this.number]
	right-fork = this.forks[(this.number + 1) % this.number]
*/
typedef struct s_philosophers
{
	int					number;
	int					*death;
	int					*forks_state;
	const int			*wait;
	pthread_t			thread_id;
	suseconds_t			last_meal;
	unsigned int		meal_count;
	pthread_mutex_t		*forks_state_lock;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*print_lock;
	const t_sim_data	*sim_data;
}t_philo;

typedef enum e_philo_state
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE
}t_philo_state;

/* Core */

int			init_shared_resources(t_table_data *data, int argc, char **argv);
void		clean_shared(t_table_data *data);

int			spawn_philosophers(t_table_data *data);
void		*philosopher_routine(void *this);
int			change_state(t_philo *this, t_philo_state state);

/* Utils */

int			mutex_unlocker(t_philo *this);
int			did_i_starve(t_philo *this);
void		do_one_philo(t_philo *this);

suseconds_t	get_time(void);
int			ft_atoi(const char *nptr);
int			ft_abs(int n);
void		*ft_calloc(size_t nmemb, size_t size);

#endif