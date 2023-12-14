/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 00:59:19 by aurban            #+#    #+#             */
/*   Updated: 2023/12/15 00:06:57 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	pre_init_data(t_shared *shared, pthread_mutex_t *print_lock, \
	pthread_mutex_t *forks_state_lock)
{
	shared->death = 0;
	shared->wait = 0;
	shared->forks_state = NULL;
	shared->forks_state_lock = forks_state_lock;
	shared->forks = NULL;
	shared->print_lock = print_lock;
	shared->philosophers_id = NULL;
}

static void	print_invalid_input(void)
{
	write(1, "Arguments\n\t"
		"number_of_philosophers\n\t"
		"time_to_die   (in mili-seconds)\n\t"
		"time_to_eat   (in mili-seconds)\n\t"
		"time_to_sleep (in mili-seconds)\n\n\t"
		"(optional)number_of_times_each_philosopher_must_eat\n\t"
		"  will end the program once each philosophers ate enough\n", 246);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t	print_lock;
	pthread_mutex_t	forks_state_lock;
	t_shared		shared;
	int				error;
	int				i;

	if (argc < 5 || argc > 6)
		return (print_invalid_input(), 0);
	pre_init_data(&shared, &print_lock, &forks_state_lock);
	error = init_shared_resources(&shared, argc, argv);
	if (error < 0)
		return (1);
	else if (error)
		return (clean_shared(&shared), 0);
	if (spawn_philosophers(&shared))
		return (clean_shared(&shared), 0);
	i = 0;
	while (i < shared.sim_data.philo_count)
		pthread_join(shared.philosophers_id[i++], NULL);
	return (clean_shared(&shared), 0);
}
