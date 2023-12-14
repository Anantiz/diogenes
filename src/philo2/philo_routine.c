/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/15 00:06:59 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	eat_then_sleep(t_philo *this)
{
	
	// EAT
	if (change_state(this, EAT))
		return (1) ;
	this->last_meal = get_time();
	this->meal_count++;
	if (ft_usleep(this, this->shared->sim_data.time_to_eat))
		return (1);
	if (fork_unlocker(this))
		return (1);

	// SLEEP
	if (this->shared->sim_data.time_to_sleep > 0)
	{
		if (change_state(this, SLEEP))
			return (1);
		if (ft_usleep(this, this->shared->sim_data.time_to_sleep))
			return (1);
	}
	return (0);
}

/*
Loop:
	if should_die:
		return & end
	if can_eat:
		eat
	else
		think
*/
static void	routine_loop(t_philo *this)
{
	int			status;
	suseconds_t	think_time;

	while (this->meal_count < this->shared->sim_data.meal_max)
	{
		status = get_forks(this);
		if (!status)
		{
			// EAT
			if (eat_then_sleep(this))
				break ;
		}
		else if (status == STARVED)
		{
			// DIE
			change_state(this, DIE);
			break ;
		}
		else
			break ;

		// THINK
		think_time = ((this->shared->sim_data.time_to_die - \
			this->shared->sim_data.time_to_eat * 2) - \
			(this->shared->sim_data.time_to_sleep));
		// Thinks for at-least think_time
		if (think_time > 0)
		{
			if (change_state(this, THINK))
				break ;
			if (ft_usleep(this, think_time))
				break ;
		}
	}
	return ;
}

void	*philosopher_routine(void *this_)
{
	t_philo	*this;

	this = this_;
	// HANDLES SPECIAL CASES
	if (this->shared->sim_data.meal_max == 0)
		return (free(this), NULL);
	if (this->shared->sim_data.philo_count == 1)
		return (do_one_philo(this), free(this), NULL);
	
	// SYNCHRONISE
	while (this->shared->wait)
		;
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
