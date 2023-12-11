/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:30:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/11 12:13:48 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Initialize the philosopher struct */
static t_philo	*init_philosopher(t_table_data *data, int i)
{
	t_philo		*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->number = i;
	philo->meal_count = 0;
	philo->forks = data->forks;
	philo->print_lock = data->print_lock;
	philo->sim_data = (const t_sim_data *)&data->sim_data;
	philo->death = &data->death;
	philo->forks_state = data->forks_state;
	philo->forks_state_lock = data->forks_state_lock;
	philo->last_meal = get_time();
	return (philo);
}

/* Spawns philo_count threads */
int	spawn_philosophers(t_table_data *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < data->sim_data.philo_count)
	{
		philo = init_philosopher(data, i);
		if (!philo)
			return (-1);
		if (pthread_create(&philo->thread_id, NULL, philosopher_routine, philo))
			return (-2);
		data->philosophers_id[i] = philo->thread_id;
		i++;
	}
	return (0);
}
