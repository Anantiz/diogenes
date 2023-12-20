/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 20:56:11 by aurban            #+#    #+#             */
/*   Updated: 2023/12/20 18:12:46 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	pre_init_data(t_shared *shared)
{
	shared->death = 0;
	shared->wait = 0;
	shared->pid_list = NULL;
	shared->forks_count = sem_open(SEM_FORK, O_CREAT);
	if (shared->forks_count == SEM_FAILED)
	{
		write(2, "Semaphore error, abort\n", 23);
		exit(-1);
	}
}

static void	print_invalid_input(void)
{
	write(1, "Arguments\n\t"
		"number_of_philosophers\n\t"
		"time_to_die   (in mili-seconds)\n\t"
		"time_to_eat   (in mili-seconds)\n\t"
		"time_to_sleep (in mili-seconds)\n\n\t"
		"(optional)number_of_times_each_philosopher_must_eat\n\t"
		"  will end the program once each philosophers ate enough\n\n"
		"Any invalid arguments will be assumed to be 0\n", 292);
}

static void	handles_eop(t_shared *shared)
{
	int		i;
	int		status;
	pid_t	dead_pid;

	dead_pid = waitpid(-1, &status, 0);
	i = 0;
	while (i < shared->sim_data.philo_count)
	{
		if (kill(shared->pid_list[i], SIGKILL))
		{
			write(2, "Some weird error occured so i died, sry\n", 40);
			return ;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_shared	shared;
	int			status;
	int			i;

	if (argc < 5 || argc > 6)
		return (print_invalid_input(), 0);
	pre_init_data(&shared);
	status = init_shared_resources(&shared, argc, argv);
	if (status < 0)
		return (1);
	else if (status)
		return (clean_shared(&shared), 0);
	if (spawn_philosophers(&shared))
		return (clean_shared(&shared), 0);
	handles_eop(&shared);
	return (clean_shared(&shared), 0);
}
