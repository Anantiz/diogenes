/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/11 12:23:46 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Tries to lock both right and left fork for the calling thread */
static int	take_forks(t_philo *this)
{
	int	error;
	
	error = pthread_mutex_lock(&this->forks[this->number]);
	if (error)
	{
		write(2, MUTEX_LOCK_ERROR, MLE_LEN);
		return (-1);
	}
	if (change_state(this, FORK))
		return (-1);
	error = pthread_mutex_lock(&this->forks[(this->number + 1) \
		% this->sim_data->philo_count]);
	if (error)
	{
		pthread_mutex_unlock(&this->forks[this->number]);
		write(2, MUTEX_LOCK_ERROR, MLE_LEN);
		return (-1);
	}
	if (change_state(this, FORK))
		return (mutex_unlocker(this), -1);
	return (0);
}

//Check if forks are available
static int	get_forks(t_philo *this)
{
	int	error;

	while (1)
	{
		error = pthread_mutex_lock(this->forks_state_lock);
		if (error)
		{
			write(2, MUTEX_LOCK_ERROR, MLE_LEN);
			return (-1);
		}
		if (this->forks_state[this->number] == 0 && this->forks_state\
			[(this->number + 1) % this->sim_data->philo_count] == 0)
		{
			this->forks_state[this->number] = 1;
			this->forks_state[(this->number + 1) % \
				this->sim_data->philo_count] = 1;
			pthread_mutex_unlock(this->forks_state_lock);
			return (take_forks(this));
		}
		pthread_mutex_unlock(this->forks_state_lock);
		if (did_i_starve(this))
			return (STARVED);
		usleep(1);
	}
	return (0);
}

void	*philosopher_routine(void *this_)
{
	t_philo	*this;
	int		error;

	this = this_;
	while (this->meal_count < this->sim_data->meal_max)
	{
		/* Thinks a.k.a wait mode*/
		error = change_state(this, THINK);
		if (error)
			break ;
		error = get_forks(this);
		if (!error)									// Eats
		{
			if (did_i_starve(this))
			{
				mutex_unlocker(this);
				change_state(this, DIE);
				break ;
			}
			if (change_state(this, EAT))
				break ;
			usleep(this->sim_data->time_to_eat);	// Dinner Time !
			mutex_unlocker(this);
			this->last_meal = get_time();
			this->meal_count++;
			if (change_state(this, SLEEP))
				break ;
			usleep(this->sim_data->time_to_sleep);	// Goes to sleep once done
		}
		else if (error == STARVED)					// Dies if starving
		{
			change_state(this, DIE);
			break ;
		}
		else
			break ;
	}
	return (free(this), NULL);
}
