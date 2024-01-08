/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:49:42 by aurban            #+#    #+#             */
/*   Updated: 2024/01/08 04:40:57 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	did_i_starve(t_philo *this)
{
	if (get_time() > this->starvation_time)
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
		return (a);
	return (b);
}

void	ft_usleep(t_philo *this, suseconds_t t)
{
	suseconds_t		wait_target;
	suseconds_t		time_now;

	time_now = get_time();
	usleep((int)get_sleep_time(t, this->starvation_time - time_now));
	wait_target = time_now + t;
	while (1)
	{
		time_now = get_time();
		if (time_now > this->starvation_time)
			change_state(this, DIE);
		else if (time_now >= wait_target)
			return ;
	}
}
