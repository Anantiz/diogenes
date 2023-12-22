/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:30:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/18 17:23:26 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Initialize the philosopher struct */
static t_philo	*init_philosopher(t_shared *shared, int i)
{
	t_philo		*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->number = i;
	philo->meal_count = 0;
	philo->shared = shared;
	return (philo);
}

/* Spawns philo_count threads */
int	spawn_philosophers(t_shared *shared)
{
	t_philo	*philo;
	int		i;

	i = 0;
	pthread_mutex_lock(shared->print_lock);
	shared->wait = 1;
	pthread_mutex_unlock(shared->print_lock);
	while (i < shared->sim_data.philo_count)
	{
		philo = init_philosopher(shared, i);
		if (!philo)
			return (-1);
		if (pthread_create(&shared->philosophers_id[i], \
			NULL, philosopher_routine, philo))
			return (-2);
		i++;
	}
	shared->sim_data.start_time = get_time();
	pthread_mutex_lock(shared->print_lock);
	shared->wait = 0;
	pthread_mutex_unlock(shared->print_lock);
	return (0);
}
