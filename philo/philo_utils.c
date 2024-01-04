/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:49:42 by aurban            #+#    #+#             */
/*   Updated: 2024/01/04 18:23:42 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Handles the special case where there is only one philosopher
void	do_one_philo(t_philo *this)
{
	if (change_state(this, FORK))
		return ;
	usleep(this->shared->sim_data.time_to_die);
	change_state(this, DIE);
	return ;
}

static suseconds_t	get_sleep_time(suseconds_t a, suseconds_t b)
{
	if (a < b)
		return (a - 512);
	return (b - 512);
}

int	ft_usleep(t_philo *this, suseconds_t t)
{
	suseconds_t		wait_target;
	suseconds_t		time_now;
	struct timeval	var;

	time_now = get_time();
	wait_target = time_now + t;
	usleep((int)get_sleep_time(t, this->starvation_time - time_now));
	while (1)
	{
		gettimeofday(&var, NULL);
		time_now = var.tv_usec + (var.tv_sec * 1000000);
		if (time_now >= this->starvation_time)
		{
			change_state(this, DIE);
			return (STARVED);
		}
		else if (time_now >= wait_target)
			return (0);
	}
}
