/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:15:17 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 04:44:59 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Parse and Init the read-only simulation data */
static int	init_simu_data(t_sim_data *sim_data, int argc, char **argv)
{
	sim_data->philo_count = ft_atoi(argv[1]);
	if (sim_data->philo_count <= 0)
		return (printf("number_of_philosophers must be a positive non-zero"
				"integer\n"));
	sim_data->time_to_die = ft_abs(ft_atoi(argv[2]));
	sim_data->time_to_eat = ft_abs(ft_atoi(argv[3]));
	sim_data->time_to_sleep = ft_abs(ft_atoi(argv[4]));
	if (argc >= 5)
		sim_data->meal_count_max = ft_abs(ft_atoi(argv[4]));
	else
		sim_data->meal_count_max = -1;
	return (0);
}

/* Parses console arguments and init shared resources */
int	init_shared_resources(t_table_data *data, int argc, char **argv)
{
	int	i;

	if (init_simu_data(&data->sim_data, argc, argv))
		return (1);
	data->forks = ft_calloc(data->sim_data.philo_count,
			sizeof(pthread_mutex_t));
	if (!data->forks)
		return (-1);
	i = 0;
	while (i < data->sim_data.philo_count)
		if (pthread_mutex_init(&data->forks[i++], NULL))
			return (printf("Mutex init failure, abort\n"));
	data->print_right = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!data->print_right)
		return (-1);
	if (pthread_mutex_init(data->print_right, NULL))
		return (printf("Mutex init failure, abort\n"));
	return (0);
}
