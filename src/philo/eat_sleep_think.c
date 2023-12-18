/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:39:20 by aurban            #+#    #+#             */
/*   Updated: 2023/12/18 13:39:39 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	eat_then_sleep(t_philo *this)
{

	if (change_state(this, EAT))
		return (release_forks(this) * 0 + 1) ;
	this->last_meal = get_time();
	this->meal_count++;
	if (ft_usleep(this, this->shared->sim_data.time_to_eat))
		return (1);
	if (release_forks(this))
		return (1);

	if (this->shared->sim_data.time_to_sleep > 0)
	{
		if (change_state(this, SLEEP))
			return (1);
		if (ft_usleep(this, this->shared->sim_data.time_to_sleep))
			return (1);
	}
	return (0);
}

int	think(t_philo *this)
{
	suseconds_t	think_time;

	think_time = ((this->shared->sim_data.time_to_die - \
		this->shared->sim_data.time_to_eat * 2) - \
		(this->shared->sim_data.time_to_sleep));
	if (think_time > 0)
	{
		if (change_state(this, THINK))
			return (-1);
		if (ft_usleep(this, think_time))
			return (-1);
	}
	return (0);
}