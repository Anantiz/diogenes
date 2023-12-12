/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   soup_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 17:03:35 by aurban            #+#    #+#             */
/*   Updated: 2023/12/12 17:12:47 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	take_forks(t_philo *this)
{
	int	error;
	
	error =);

	else
	{
	}

	if (change_state(this, FORK))
		return (-1);
	error = pthread_mutex_lock(&this->forks[(this->number + 1) \
		% this->sim_data->philo_count]);
	if (error)
	{
		pthread_mutex_unlock(&this->forks[this->number]);
		write(2, MUTEX_LOCK_ERROR"01\n", MLE_LEN + 3);
		return (-1);
	}
	if (change_state(this, FORK))
		return (mutex_unlocker(this) - 100);
	return (0);
}

//Check if forks are available
static int	get_forks(t_philo *this)
{
	int	error;
	int	i_have_right;
	int	i_have_left;

	i_have_right = 0;
	i_have_left = 0;
	while (1)
	{
		if (pthread_mutex_lock(this->forks_state_lock))
		{
			write(2, MUTEX_LOCK_ERROR"04\n", MLE_LEN + 3);
			return (-1);
		}


		// Left fork
		if (this->forks_state[this->number] == 0)
		{
			i_have_left = 1;
			this->forks_state[this->number] = 1;
			if (pthread_mutex_lock(&this->forks[this->number]))
			{
				write(2, MUTEX_LOCK_ERROR"89\n", MLE_LEN + 3);
				if (i_have_right)
				{
					this->forks_state[(this->number + 1) % this->sim_data->philo_count] = 0;
					pthread_mutex_unlock(&this->forks[(this->number + 1) % this->sim_data->philo_count]);
				}
				return (-1);
			}
			pthread_mutex_unlock(this->forks_state_lock);
		}
		
		// Right fork
		if (this->forks_state[(this->number + 1) % this->sim_data->philo_count] == 0)
		{
			i_have_right = 1;
			this->forks_state[(this->number + 1) % this->sim_data->philo_count] = 1;
			if (pthread_mutex_lock(&this->forks[(this->number + 1) % this->sim_data->philo_count]))
			{
				if (i_have_left)
				{
					this->forks_state[this->number] = 0;
					pthread_unlcok(&this->forks[this->number]);
				}
				write(2, MUTEX_LOCK_ERROR"88\n", MLE_LEN + 3);
				return (-1);
			}
			pthread_mutex_unlock(this->forks_state_lock);
		}

		pthread_mutex_unlock(this->forks_state_lock);
		if (did_i_starve(this))
			return (STARVED);
	}
	return (0);
}