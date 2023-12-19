/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:39:20 by aurban            #+#    #+#             */
/*   Updated: 2023/12/19 14:20:26 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	release_forks_all(t_philo *this)
{
	int	status;

	status = 0;
	if (!pthread_mutex_lock(this->shared->one_to_rule_them_all))
	{
		release_forks_state(this);
		if (release_forks_mutex(this))
			status = -1;
	}
	else
		return (-1);
	pthread_mutex_unlock(this->shared->one_to_rule_them_all);
	return (status);
}

int	eat_then_sleep(t_philo *this)
{
	if (change_state(this, EAT))
	{
		printf("AAA2\n");
		release_forks_all(this);
		return (-1);
	}
	this->last_meal = get_time();
	this->meal_count++;
	if (ft_usleep(this, this->shared->sim_data.time_to_eat))
		return (-1);
	if (release_forks_all(this))
	{
		printf("AAA\n");
		return (-1);
	}
	if (change_state(this, SLEEP))
		return (-1);
	if (ft_usleep(this, this->shared->sim_data.time_to_sleep))
		return (-1);
	return (0);
}

int	think(t_philo *this)
{
	suseconds_t	think_time;

	think_time = ((this->shared->sim_data.time_to_die - \
		this->shared->sim_data.time_to_eat * 2) - \
		(this->shared->sim_data.time_to_sleep)) - 4;
	if (think_time > 0)
	{
		if (change_state(this, THINK))
			return (-1);
		if (ft_usleep(this, think_time))
			return (-1);
	}
	return (0);
}
