/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2023/12/18 17:20:34 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//if Status == True: lock, else:unlock
int	forks_lock(t_philo *this, int status)
{
	if (status)
	{
		if (pthread_mutex_lock(&this->shared->forks_lock[this->number]))
			return (-1);
		if (pthread_mutex_lock(&this->shared->forks_lock[(this->number + 1) % \
			this->shared->sim_data.philo_count]))
		{
			pthread_mutex_unlock(&this->shared->forks_lock[this->number]);
			return (-1);
		}
	}
	else
	{
		pthread_mutex_unlock(&this->shared->forks_lock[this->number]);
		pthread_mutex_unlock(&this->shared->forks_lock[(this->number + 1) % \
				this->shared->sim_data.philo_count]);
	}
	return (0);
}

int	release_forks(t_philo *this)
{
	if (forks_lock(this, 1))
		return (-1);
	this->shared->forks_state[this->number] = 0;
	this->shared->forks_state[(this->number + 1) % \
	this->shared->sim_data.philo_count] = 0;
	forks_lock(this, 0);
	return (0);
}

int	get_forks(t_philo *this)
{
	while (1)
	{
		if (did_i_starve(this))
			return (STARVED);
		if (forks_lock(this, 1))
			return (-1);
		if (this->shared->forks_state[this->number] == 0 && \
			this->shared->forks_state[(this->number + 1) % \
			this->shared->sim_data.philo_count] == 0)
		{
			this->shared->forks_state[this->number] = 1;
			this->shared->forks_state[(this->number + 1) % \
				this->shared->sim_data.philo_count] = 1;
			forks_lock(this, 0);
			return (0);
		}
		forks_lock(this, 0);
	}
}
