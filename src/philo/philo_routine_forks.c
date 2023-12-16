/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_forks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2023/12/16 18:00:15 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	take_forks_error(t_philo *this, int status)
{
	if (status == 1)
		write (2, "AAAAA MUTEX ERROR (philo_routine_forks 01)\n", 44);
	else if (status == 2)
		write (2, "AAAAA MUTEX ERROR (philo_routine_forks 02)\n", 44);
	if (!pthread_mutex_lock(this->shared->forks_state_lock))
	{
		this->shared->forks_state[this->number] = 0;
		this->shared->forks_state[(this->number + 1) % \
			this->shared->sim_data.philo_count] = 0;
		pthread_mutex_unlock(this->shared->forks_state_lock);
	}
}

/*
Locks Fork (i):
	error--> return
Locks Fork (i + 1):
	error--> unlock-i; return
*/
static int	take_forks(t_philo *this)
{

	// LEFT Fork
	if (pthread_mutex_lock(&this->shared->forks[this->number]))
	{
		write(2, MUTEX_LOCK_ERROR"02\n", MLE_LEN + 3);
		return (take_forks_error(this, 1), -1);
	}
	if (change_state(this, FORK))
	{
		pthread_mutex_unlock(&this->shared->forks[this->number]);
		return (take_forks_error(this, 0), -1);
	}

	// Right Fork
	if (pthread_mutex_lock(&this->shared->forks[(this->number + 1) % \
		this->shared->sim_data.philo_count]))
	{
		pthread_mutex_unlock(&this->shared->forks[this->number]);
		write(2, MUTEX_LOCK_ERROR"01\n", MLE_LEN + 3);
		return (take_forks_error(this, 2), -1);
	}
	if (change_state(this, FORK))
		return (take_forks_error(this, 0), (fork_unlocker(this) * 0) - 1);
	return (0);
}

int	get_forks(t_philo *this)
{
	while (1)
	{
		if (did_i_starve(this))
			return (STARVED);
		if (pthread_mutex_lock(this->shared->forks_state_lock))
		{
			write(2, MUTEX_LOCK_ERROR"04\n", MLE_LEN + 3);
			return (-1);
		}
		if (this->shared->forks_state[this->number] == 0 && this->shared->forks_state \
			[(this->number + 1) % this->shared->sim_data.philo_count] == 0)
		{
			this->shared->forks_state[this->number] = 1;
			this->shared->forks_state[(this->number + 1) % \
				this->shared->sim_data.philo_count] = 1;
			pthread_mutex_unlock(this->shared->forks_state_lock);
			return (take_forks(this));
		}
		pthread_mutex_unlock(this->shared->forks_state_lock);
	}
}
