/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2024/01/04 18:03:02 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	shall_take_forks(t_philo *this)
{
	if (this->shared->forks_state[this->number] > -1 && \
		this->shared->forks_state[this->next_fork] > -1)
	{
		this->shared->forks_state[this->number] = this->number;
		this->shared->forks_state[this->next_fork] = this->number;
		return (1);
	}
	return (0);
}

void	release_forks_state(t_philo *this)
{
	this->shared->forks_state[this->number] = 0;
	this->shared->forks_state[this->next_fork] = 0;
}

/*
	Shall alway release the mutex
*/
void	release_forks_mutex(t_philo *this)
{
	pthread_mutex_unlock(&this->shared->forks_lock[this->number]);
	pthread_mutex_unlock(&this->shared->forks_lock[this->next_fork]);
}

/*
	Shall Alway either lock both or None.
*/
static int	take_forks_mutex(t_philo *this)
{
	if (pthread_mutex_lock(&this->shared->forks_lock[this->number]))
		return (MUTEX_FAILURE);
	if (change_state(this, FORK))
		return (STARVED);
	if (pthread_mutex_lock(&this->shared->forks_lock[this->next_fork]))
	{
		pthread_mutex_unlock(&this->shared->forks_lock[this->number]);
		return (MUTEX_FAILURE);
	}
	if (change_state(this, FORK))
		return (STARVED);
	return (0);
}

/*
Look-up forks:
	if (avaliable)
		lock forks
close look-up forks */
int	get_forks(t_philo *this)
{
	while (1)
	{
		if (get_time() > this->starvation_time)
			return (STARVED);
		if (shall_take_forks(this))
		{
			if (take_forks_mutex(this))
			{
				release_forks_state(this);
				return (MUTEX_FAILURE);
			}
			return (0);
		}
	}
}
