/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:30:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 06:20:49 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_philo	*init_philosopher(t_table_data *data, int i)
{
	t_philo		*philo;
	suseconds_t	time_now;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	time_now = get_time();
	philo = {i, 0, time_now, data->forks, data->print_lock, 0, data->sim_data};
	return (philo);
}

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
		if (pthread_create(philo->thread_id, NULL, philosopher_routine, philo))
			return (-1);
		i++;
	}	
}
