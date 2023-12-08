/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 16:12:58 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Tries to lock both right and left fork for the calling thread */
static int	get_forks(t_philo *this)
{
	int	error;

	if (get_time() - this->last_meal >= this->sim_data->time_to_die)
	{
		pthread_mutex_unlock(&this->forks[this->number]);
		pthread_mutex_unlock(&this->forks[(this->number + 1) % \
			this->sim_data->philo_count]);
		return (1) ;
	}
	error = pthread_mutex_lock(&this->forks[this->number]);
	if (error)
	{
		pthread_mutex_unlock(&this->forks[this->number]);
		return (-1);
	}
	if (change_state(this, FORK))
		return (-1);
	// printf("%d: locked fork %d\n", this->number, this->number);
	error = pthread_mutex_lock(&this->forks[(this->number + 1) \
		% this->sim_data->philo_count]);
	if (error)
	{
		pthread_mutex_unlock(&this->forks[this->number]);
		// printf("%d: unlocked fork %d\n", this->number, this->number);
		pthread_mutex_unlock(&this->forks[(this->number + 1) % \
			this->sim_data->philo_count]);
		// printf("%d: unlocked fork %d\n", this->number,(this->number + 1) % \
			this->sim_data->philo_count);
		return (-1);
	}
	if (change_state(this, FORK))
		return (-1);
	// printf("%d: locked fork %d\n", this->number,(this->number + 1) % \
		this->sim_data->philo_count);
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
		{
			write(2, "ERROR Aborting code: 01\n", 25);
			break ;
		}
		error = get_forks(this);
		if (!error)									// Eats
		{
			if (get_time() - this->last_meal >= this->sim_data->time_to_die)
			{
				pthread_mutex_unlock(&this->forks[this->number]);
				// printf("%d: unlocked fork %d\n", this->number, this->number);
				pthread_mutex_unlock(&this->forks[(this->number + 1) % \
					this->sim_data->philo_count]);
				// printf("%d: unlocked fork %d\n", this->number,(this->number + 1) % \
					this->sim_data->philo_count);
				change_state(this, DIE);
				break ;
			}
			error = change_state(this, EAT);
			if (error)
			{
				write(2, "ERROR Aborting code: 02\n", 25);
				break ;
			}
			usleep(this->sim_data->time_to_eat);	// Dinner Time !
			pthread_mutex_unlock(&this->forks[this->number]);
			// printf("%d: unlocked fork %d\n", this->number, this->number);
			pthread_mutex_unlock(&this->forks[(this->number + 1) % \
				this->sim_data->philo_count]);
			// printf("%d: unlocked fork %d\n", this->number,(this->number + 1) % \
				this->sim_data->philo_count);
			this->last_meal = get_time();
			this->meal_count++;
			error = change_state(this, SLEEP);
			if (error)
			{
				write(2, "ERROR Aborting code: 03\n", 25);
				break ;
			}
			usleep(this->sim_data->time_to_sleep);	// Goes to sleep once done
		}
		else if (error == 1)						// Dies if starving
		{
			change_state(this, DIE);
			break ;
		}
		else
		{
			write(2, "ERROR Aborting code: 66\n", 25);
			break ;
		}
		fflush(stdout);
	}
	return (free(this), NULL);
}
