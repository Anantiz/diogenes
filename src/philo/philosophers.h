/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:49:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 07:18:29 by aurban           ###   ########.fr       */
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

/* Read-only after initialization */
typedef struct s_sim_data
{
	int				philo_count;
	unsigned int	meal_count_max;
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
	t_sim_data		sim_data;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	pthread_t		*philosophers_id;
}t_table_data;

/* N + 1 = (N + 1) % philo_count */
typedef struct s_philosophers
{
	int					number;
	pthread_t			thread_id;
	unsigned int		meal_count;
	suseconds_t			last_meal;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*print_lock;
	const t_sim_data	*sim_data;
}t_philo;

typedef enum e_philo_state
{
	FORK,
	EAT,
	SLEEP,
	DIE
}t_philo_state;

/* Core */

int			init_shared_resources(t_table_data *data, int argc, char **argv);
void		clean_shared(t_table_data *data);

int			spawn_philosophers(t_table_data *data);
void		*philosopher_routine(void *this);

/* Utils */

suseconds_t	get_time(void);
int			ft_atoi(const char *nptr);
int			ft_abs(int n);
void		*ft_calloc(size_t nmemb, size_t size);

#endif