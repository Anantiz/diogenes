/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:39:20 by aurban            #+#    #+#             */
/*   Updated: 2024/01/08 04:43:38 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	release_forks_all(t_philo *this)
{
	release_forks_mutex(this);
}

int	eat_then_sleep(t_philo *this)
{
	if (change_state(this, EAT))
		return (release_forks_all(this), STARVED);
	if (ft_usleep(this, this->shared->sim_data.time_to_eat))
		return (release_forks_all(this), STARVED);
	this->starvation_time = get_time() + this->shared->sim_data.time_to_die;
	this->meal_count++;
	release_forks_all(this);
	if (change_state(this, SLEEP))
		return (STARVED);
	if (ft_usleep(this, this->shared->sim_data.time_to_sleep))
		return (STARVED);
	return (0);
}

int	think(t_philo *this)
{
	suseconds_t	forced_think_duration;

	forced_think_duration = this->starvation_time - get_time() - \
		(this->shared->sim_data.time_to_eat + \
		this->shared->sim_data.time_to_eat);
	if (forced_think_duration > 0)
	{
		if (change_state(this, THINK))
			return (DEATH_VAL);
		if (ft_usleep(this, forced_think_duration))
			return (DEATH_VAL);
	}
	return (0);
}
