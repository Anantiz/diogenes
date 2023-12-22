/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:49:42 by aurban            #+#    #+#             */
/*   Updated: 2023/12/22 17:13:21 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	did_i_starve(t_philo *this)
{
	if (get_time() - this->last_meal > this->shared.sim_data.time_to_die)
		return (1);
	return (0);
}

// Handles the special case where there is only one philosopher
void	do_one_philo(t_philo *this)
{
	change_state(this, FORK);
	usleep(this->shared.sim_data.time_to_die);
	change_state(this, DIE);
	free(this);
	exit(SUB_PROCESS_SUCCESS);
}

static suseconds_t	get_sleep_time(suseconds_t a, suseconds_t b)
{
	if (a < b)
	{
		a -= 5;
		if (a < 0)
			return (0);
		return (a);
	}
	b -= 5;
	if (b < 0)
		return (0);
	return (b);
}

int	ft_usleep(t_philo *this, suseconds_t t)
{
	suseconds_t		wait_target;
	suseconds_t		time_now;
	struct timeval	var;
	suseconds_t		time_die;

	wait_target = get_time() + t;
	usleep((int)get_sleep_time(t, this->shared.sim_data.time_to_die));
	time_die = this->shared.sim_data.time_to_die + this->last_meal;
	while (1)
	{
		gettimeofday(&var, NULL);
		time_now = var.tv_usec + (var.tv_sec * 1000000);
		if (time_now > time_die)
			change_state(this, DIE);
		else if (time_now >= wait_target)
			return (0);
	}
}
