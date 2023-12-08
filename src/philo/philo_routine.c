/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 14:48:25 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Tries to lock both right and left fork for the calling thread */
static int	get_forks(t_philo *this)
{
	int	error;

	while (get_time() - this->last_meal <= this->sim_data->time_to_die)
	{
		// if (!this->forks_state[this->number] && \
		// 	!this->forks_state[(this->number + 1) % this->number])
		// {
			error = pthread_mutex_lock(&this->forks[this->number]);
			if (error)
			{
				pthread_mutex_unlock(&this->forks[this->number]);
				return (-1);
			}
			error = pthread_mutex_lock(&this->forks[(this->number + 1) \
				% this->number]);
			if (error)
			{
				pthread_mutex_unlock(&this->forks[(this->number + 1) % \
					this->number]);
				return (-1);
			}
			return (0);
		// }
	}
	return (1);
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
		if (!error)								// Eats
		{
			error = change_state(this, EAT);
			if (error)
			{
				write(2, "ERROR Aborting code: 02\n", 25);
				break ;
			}
			usleep(this->sim_data->time_to_eat);	// Dinner Time !
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
	}
	return (free(this), NULL);
}
