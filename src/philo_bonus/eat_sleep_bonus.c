/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2023/12/19 18:57:47 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	fork_unlocker(t_philo *this)
{
	if (!pthread_mutex_lock(this->shared->forks_lock))
	{
		this->shared->forks_count -= 2;
		pthread_mutex_unlock(this->shared->forks_lock);
		return (0);
	}
	else
	{
		write(2, MUTEX_LOCK_ERROR"08\n", MLE_LEN + 3);
		return (-1);
	}
}

static int	shall_i_eat(t_philo *this)
{
	if (this)
	{
		return (1);
	}
	return (0);
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
			if (pthread_mutex_lock(this->shared->forks_lock))
			{
				write(2, MUTEX_LOCK_ERROR"04\n", MLE_LEN + 3);
				return (-1);
			}
			this->shared->forks_count += 2;
			pthread_mutex_unlock(this->shared->forks_lock);
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
