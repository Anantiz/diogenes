/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resources.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:15:17 by aurban            #+#    #+#             */
/*   Updated: 2024/01/08 04:22:27 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Parse and Init the read-only simulation data */
static int	init_simu_data(t_sim_data *sim_data, int argc, char **argv)
{
	sim_data->philo_count = ft_atoi(argv[1]);
	if (sim_data->philo_count <= 0)
		return (printf("number_of_philosophers must be a positive non-zero "
				"integer\n"));
	sim_data->time_to_die = ft_abs(ft_atoi(argv[2])) * 1000;
	sim_data->time_to_eat = ft_abs(ft_atoi(argv[3])) * 1000;
	sim_data->time_to_sleep = ft_abs(ft_atoi(argv[4])) * 1000;
	if (argc > 5)
		sim_data->meal_max = ft_abs(ft_atoi(argv[5]));
	else
		sim_data->meal_max = -1;
	return (0);
}

/* Parses console arguments and init shared resources */
int	init_shared_resources(t_shared *shared, int argc, char **argv)
{
	int	i;

	if (init_simu_data(&shared->sim_data, argc, argv))
		return (1);
	if (pthread_mutex_init(shared->print_lock, NULL))
		return (-printf("Mutex init failure, abort\n"));
	shared->forks_lock = ft_calloc(shared->sim_data.philo_count + 1, \
		sizeof(pthread_mutex_t));
	if (!shared->forks_lock)
		return (-1);
	i = 0;
	while (i < shared->sim_data.philo_count)
		if (pthread_mutex_init(&shared->forks_lock[i++], NULL))
			return (printf("Mutex init failure, abort\n"));
	shared->forks_state = ft_calloc(shared->sim_data.philo_count, sizeof(int));
	if (!shared->forks_state)
		return (printf("malloc failure, abort\n"));
	shared->philosophers_id = ft_calloc(shared->sim_data.philo_count, \
		sizeof(pthread_t *));
	if (!shared->philosophers_id)
		return (-1);
	return (0);
}

void	clean_shared(t_shared *shared)
{
	int	i;

	i = 0;
	while (shared->forks_lock && i < shared->sim_data.philo_count \
		&& &shared->forks_lock[i] != NULL)
	{
		pthread_mutex_destroy(&shared->forks_lock[i++]);
	}
	free(shared->forks_lock);
	free(shared->forks_state);
	free(shared->philosophers_id);
	if (shared->print_lock)
		pthread_mutex_destroy(shared->print_lock);
}
