/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:49:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 01:25:03 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philosophers
{
	int				number;
	int				meal_count;
	long			left_fork;
	long			right_fork;
	suseconds_t		last_meal;
	pthread_mutex_t	*forks_lock;
	pthread_t		thread_id;
}t_philo;


/*
Forks will be stored as a single variable with status bytes
Each Philosopher will be entitled to a status byte
Thus the state of the forks on the table are only represented
as a status, and they do not have a representation in memory
for a philosopher to acces a fork will only require one mutex
*/
typedef struct s_table
{
	int				philo_count;
	int				meal_count_max;
	suseconds_t		time_to_die;
	suseconds_t		time_to_eat;
	suseconds_t		time_to_sleep;
	pthread_mutex_t	*forks_lock;
}t_table_data;


int			init_shared_resources(t_table_data *table_data, int argc, char **argv);


#endif