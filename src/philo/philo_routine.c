/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/11 16:02:13 by aurban           ###   ########.fr       */
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
		write(2, MUTEX_LOCK_ERROR"02\n", MLE_LEN + 3);
		return (-1);
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

	while (1)
	{
		error = pthread_mutex_lock(this->forks_state_lock);
		if (error)
		{
			write(2, MUTEX_LOCK_ERROR"04\n", MLE_LEN + 3);
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
	}
	return (0);
}

static int	eat_then_sleep(t_philo *this)
{
	if (did_i_starve(this))
	{
		mutex_unlocker(this);
		change_state(this, DIE);
		return (1) ;
	}
	if (change_state(this, EAT))
		return (1) ;
	usleep(this->sim_data->time_to_eat);	// Dinner Time !
	this->last_meal = get_time();
	this->meal_count++;
	if (mutex_unlocker(this))
		return (1) ;
	if (change_state(this, SLEEP))
		return (1) ;
	usleep(this->sim_data->time_to_sleep);	// Goes to sleep once done
	return (0);
}

static void	routine_loop(t_philo *this)
{
	int		error;

	while (this->meal_count < this->sim_data->meal_max)
	{
		if (change_state(this, THINK))
			break ;
		error = get_forks(this);
		if (!error)									// Eats
		{
			if (eat_then_sleep(this))
				break ;

		}
		else if (error == STARVED)					// Dies if starving
		{
			change_state(this, DIE);
			break ;
		}
		else
			break ;
	}
	return ;
}

void	*philosopher_routine(void *this_)
{
	t_philo	*this;

	this = this_;
	if (this->sim_data->philo_count == 1)
		return (do_one_philo(this), free(this), NULL);
	while (*(this->wait))
	{
	}
	this->last_meal = get_time();
	routine_loop(this);
	return (free(this), NULL);
}
