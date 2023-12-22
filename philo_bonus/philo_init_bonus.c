/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:30:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/22 18:12:07 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/* Initialize the philosopher struct */
static t_philo	*init_philosopher(t_shared *shared, int i)
{
	t_philo		*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->number = i;
	philo->meal_count = 0;
	philo->shared = *shared;
	return (philo);
}

static int	philo_do_thing(t_shared *shared)
{
	sem_t	*sem;

	sem = sem_open(SEM_WAIT, O_CREAT, 0644, shared->sim_data.philo_count);
	if (sem)
	{
		sem_close(sem);
		return (420);
	}
	else
		return (-1);
}

// Spawns philo_count processes
int	spawn_philosophers(t_shared *shared)
{
	t_philo	*philo;
	int		pid;
	int		i;

	i = 0;
	while (i < shared->sim_data.philo_count)
	{
		philo = init_philosopher(shared, i);
		if (!philo)
			return (-1);
		pid = fork();
		if (pid == -1)
			return (-1);
		if (pid == 0)
			philosopher_routine(philo);
		else
		{
			free(philo);
			shared->pid_list[i] = pid;
			i++;
		}
	}
	return (philo_do_thing(shared));
}
