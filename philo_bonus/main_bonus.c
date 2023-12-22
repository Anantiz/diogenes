/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 20:56:11 by aurban            #+#    #+#             */
/*   Updated: 2023/12/22 17:31:34 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	pre_init_data(t_shared *shared)
{
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_WAIT);
	sem_unlink(SEM_PRINT);
	shared->wait = 0;
	shared->pid_list = NULL;
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

// Handles End Of Process
static void	handles_eop(t_shared *shared)
{
	int		i;
	int		status;
	pid_t	dead_pid;

	dead_pid = waitpid(-1, &status, 0);
	if (dead_pid == -1)
	{
		printf("AAAAA FIRE EVERYWHERE !!\n"
		"\tstatus: %d\terrno: %d\n", status, errno);
		return ;
	}
	i = 0;
	while (i < shared->sim_data.philo_count)
	{
		if (dead_pid != shared->pid_list[i] && kill(shared->pid_list[i], SIGKILL))
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

	if (argc < 5 || argc > 6)
		return (print_invalid_input(), 0);
	pre_init_data(&shared);
	status = init_shared_resources(&shared, argc, argv);
	if (status < 0)
		return (1);
	else if (status)
		return (clean_shared(&shared), 0);
	status = spawn_philosophers(&shared);
	handles_eop(&shared);
	return (clean_shared(&shared), 0);
}
