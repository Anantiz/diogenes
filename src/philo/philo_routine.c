/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/18 13:39:15 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	routine_loop(t_philo *this)
{
	int			status;

	while (this->meal_count < this->shared->sim_data.meal_max)
	{
		status = get_forks(this);
		if (!status)
		{
			if (eat_then_sleep(this))
				break ;
		}
		else if (status == STARVED)
		{
			change_state(this, DIE);
			break ;
		}
		else
			break ;
		if (think(this))
			break ;
	}
	return ;
}

void	*philosopher_routine(void *this_)
{
	t_philo	*this;
	int		wait;

	this = this_;

	// WAITS
	wait = 1;
	while (wait)
	{
		pthread_mutex_lock(this->shared->print_lock);
		if (this->shared->wait == 0)
		{
			pthread_mutex_unlock(this->shared->print_lock);
			break ;
		}
		pthread_mutex_unlock(this->shared->print_lock);
	}

	// HANDLES SPECIAL CASES
	if (this->shared->sim_data.meal_max == 0)
		return (free(this), NULL);
	if (this->shared->sim_data.philo_count == 1)
		return (do_one_philo(this), free(this), NULL);

	// SYNCHRONISE
	if ((this->number + 1) % 2 == 0)
	{
		if (this->shared->sim_data.philo_count % 2 == 1 && \
			this->number + 1 == this->shared->sim_data.philo_count)
			ft_usleep(NULL, this->shared->sim_data.time_to_eat);
		else
			ft_usleep(NULL, (this->shared->sim_data.time_to_eat / 2));
	}

	// START ROUTINE
	this->last_meal = get_time();
	routine_loop(this);
	return (free(this), NULL);
}