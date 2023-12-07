/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:49:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/07 22:04:18 by aurban           ###   ########.fr       */
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
	unsigned long	last_meal;
	pthread_t		thread_id;
}t_philo;

typedef struct s_table
{
	int				philo_count;
	int				meal_count_max;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	pthread_mutex_t	forks;
}t_table;


#endif