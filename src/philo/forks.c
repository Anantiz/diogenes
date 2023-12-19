/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2023/12/19 19:03:53 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	shall_take_forks(t_philo *this)
{
	if (this->shared->forks_state[this->number] == 0 && \
		this->shared->forks_state[(this->number + 1) % \
		this->shared->sim_data.philo_count] == 0)
	{
		this->shared->forks_state[this->number] = 1;
		this->shared->forks_state[(this->number + 1) % \
		this->shared->sim_data.philo_count] = 1;
		return (1);
	}
	return (0);
}

void	release_forks_state(t_philo *this)
{
	this->shared->forks_state[this->number] = 0;
	this->shared->forks_state[(this->number + 1) % \
	this->shared->sim_data.philo_count] = 0;
}

/*
	Shall alway release the mutex
*/
int	release_forks_mutex(t_philo *this)
{
	if (pthread_mutex_unlock(&this->shared->forks_lock[this->number]))
		return (-1);
	if (pthread_mutex_unlock(&this->shared->forks_lock[(this->number + 1) % \
		this->shared->sim_data.philo_count]))
		return (-1);
	return (0);
}

/*
	Shall Alway either lock both or None.
*/
static int	take_forks_mutex(t_philo *this)
{
	if (pthread_mutex_lock(&this->shared->forks_lock[this->number]))
		return (-1);
	if (change_state(this, FORK))
		return (-1);
	if (pthread_mutex_lock(&this->shared->forks_lock[(this->number + 1) % \
		this->shared->sim_data.philo_count]))
	{
		pthread_mutex_unlock(&this->shared->forks_lock[this->number]);
		return (-1);
	}
	if (change_state(this, FORK))
		return (-1);
	return (0);
}

/*
Look-up forks:
	if (avaliable)
		lock forks
close look-up forks */
int	get_forks(t_philo *this)
{
	int	status;

	status = 0;
	while (1)
	{
		if (did_i_starve(this))
			return (STARVED);
		pthread_mutex_lock(this->shared->one_to_rule_them_all);
		if (shall_take_forks(this))
		{
			if (take_forks_mutex(this))
			{
				release_forks_state(this);
				status = -1;
			}
			pthread_mutex_unlock(this->shared->one_to_rule_them_all);
			break ;
		}
		pthread_mutex_unlock(this->shared->one_to_rule_them_all);
	}
	return (status);
}
