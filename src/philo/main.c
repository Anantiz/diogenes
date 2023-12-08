/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 00:59:19 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 14:17:20 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	t_table_data	data;
	int				error;
	int				i;

	if (argc < 5 || argc > 6)
		return (print_invalid_input(), 0);
	data.forks = NULL;
	data.print_lock = NULL;
	error = init_shared_resources(&data, argc, argv);
	if (error < 0)
		return (1);
	else if (error)
		return (clean_shared(&data), 0);
	printf("data.philo_count:   \t%d\n", data.sim_data.philo_count);
	printf("data.time_to_die:   \t%ld\n", data.sim_data.time_to_die);
	printf("data.time_to_eat:   \t%ld\n", data.sim_data.time_to_eat);
	printf("data.time_to_sleep: \t%ld\n", data.sim_data.time_to_sleep);
	// Spawns the philosophers threads
	if (spawn_philosophers(&data))
		return (clean_shared(&data), 0);
	// Create a thread status for error handling if needed
	i = 0;
	while (i < data.sim_data.philo_count)
		pthread_join(data.philosophers_id[i++], NULL);
	return (clean_shared(&data), 0);
}
