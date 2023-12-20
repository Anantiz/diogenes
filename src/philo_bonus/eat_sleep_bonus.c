/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2023/12/20 18:13:18 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	fork_unlocker(t_philo *this)
{
	/*
	sem_post()
	*/
}

int	get_forks(t_philo *this)
{
	while (1)
	{
		if (did_i_starve(this))
		{
			change_state(this, DIE);
			return (DEATH_VAL);
		}
		if (shall_i_eat(this))
		{
			this->shared->forks_count -= 2;
			return (change_state(this, FORK));
		}
	}
}

int	eat_then_sleep(t_philo *this)
{
	if (change_state(this, EAT))
	{
		fork_unlocker(this);
		return (-1);
	}
	this->last_meal = get_time();
	this->meal_count++;
	if (ft_usleep(this, this->shared->sim_data.time_to_eat) == DEATH_VAL)
	{
		fork_unlocker(this);
		return (DEATH_VAL);
	}
	if (fork_unlocker(this))
		return (-1);
	if (this->shared->sim_data.time_to_sleep > 0)
	{
		if (change_state(this, SLEEP) == DEATH_VAL)
			return (DEATH_VAL);
		if (ft_usleep(this, this->shared->sim_data.time_to_sleep) == DEATH_VAL)
			return (DEATH_VAL);
	}
	return (0);
}
