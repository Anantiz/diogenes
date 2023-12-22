/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2023/12/22 18:08:00 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	fork_unlocker(t_philo *this)
{
	sem_post(this->forks_count);
	return (sem_post(this->forks_count));
}

int	get_forks(t_philo *this)
{
	if (did_i_starve(this))
		change_state(this, DIE);
	if (!sem_wait(this->forks_count))
	{
		change_state(this, FORK);
		if (!sem_wait(this->forks_count))
			return (change_state(this, FORK), 0);
	}
	return (-1);
}

int	eat_then_sleep(t_philo *this)
{
	change_state(this, EAT);
	this->last_meal = get_time();
	this->meal_count++;
	ft_usleep(this, this->shared.sim_data.time_to_eat);
	if (fork_unlocker(this))
		return (-1);
	if (this->shared.sim_data.time_to_sleep > 0)
	{
		change_state(this, SLEEP);
		ft_usleep(this, this->shared.sim_data.time_to_sleep);
	}
	return (0);
}

void	think(t_philo *this)
{
	long	think_time;

	think_time = this->shared.sim_data.time_to_eat - \
		this->shared.sim_data.time_to_sleep - 5;
	if (this->shared.sim_data.philo_count % 2)
		think_time += this->shared.sim_data.time_to_eat;
	if (think_time > 0)
	{
		change_state(this, THINK);
		ft_usleep(this, think_time);
	}
	if (did_i_starve(this))
		change_state(this, DIE);
}
