/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:30:48 by aurban            #+#    #+#             */
/*   Updated: 2023/12/20 18:02:13 by aurban           ###   ########.fr       */
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
	philo->shared = shared;
	return (philo);
}

/*
Spawns philo_count processes
*/
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
			return (philosopher_routine(philo));
		else
		{
			free(philo);
			shared->pid_list[i] = pid;
			i++;
		}
	}
	return (0);
}

/*
sem_open
sem_close
sem_post
sem_wait
sem_unlink
*/


/*
routine()
{
	loop:
		if (some_condition)
			break ;
	return (1)
}

foo2()
{
	// Creates multiple porcess
	loop:
		pid = fork()
		if (pid == 0)
			return (routine())

	return (0)
}

foo1()
{
	// Exit the program once a process returns
	if (foo2())
		printf("A")
		exit()
}
*/
