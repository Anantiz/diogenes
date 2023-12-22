/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resources_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:15:17 by aurban            #+#    #+#             */
/*   Updated: 2023/12/22 17:33:55 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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
	init_simu_data(&shared->sim_data, argc, argv);
	shared->pid_list = ft_calloc(shared->sim_data.philo_count, \
		sizeof(pthread_t *));
	if (!shared->pid_list)
		return (-1);
	shared->forks_count = sem_open(SEM_FORK, O_CREAT, 0644, \
		shared->sim_data.philo_count);
	if (!shared->forks_count)
	{
		write(2, "Semaphore error, abort\n", 23);
		return (-1);
	}
	sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	return (0);
}

void	clean_shared(t_shared *shared)
{
	free(shared->pid_list);
	sem_close(shared->forks_count);
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_WAIT);
	sem_unlink(SEM_PRINT);
}
