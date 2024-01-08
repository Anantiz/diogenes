/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 23:32:45 by aurban            #+#    #+#             */
/*   Updated: 2024/01/08 04:45:31 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
	Shall alway release the mutex
*/
void	release_forks_mutex(t_philo *this)
{
	pthread_mutex_unlock(&this->shared->forks_lock[this->number]);
	pthread_mutex_unlock(&this->shared->forks_lock[this->next_fork]);
}

static int	take_forks_mutex_odd(t_philo *this)
{
	if (pthread_mutex_lock(&this->shared->forks_lock[this->number]))
		return (MUTEX_FAILURE);
	if (change_state(this, FORK))
		return (((void)pthread_mutex_unlock(&this->shared->forks_lock \
			[this->number])), STARVED);
	if (pthread_mutex_lock(&this->shared->forks_lock[this->next_fork]))
	{
		pthread_mutex_unlock(&this->shared->forks_lock[this->number]);
		return (MUTEX_FAILURE);
	}
	if (change_state(this, FORK))
		return (((void)pthread_mutex_unlock(&this->shared->forks_lock \
		[this->number])), (void)(pthread_mutex_unlock \
		(&this->shared->forks_lock[this->next_fork])), MUTEX_FAILURE);
	return (0);
}

/*
	Shall Alway either lock both or None.
*/
static int	take_forks_mutex(t_philo *this)
{
	if (this->number % 2 == 0)
	{
		if (pthread_mutex_lock(&this->shared->forks_lock[this->next_fork]))
			return (MUTEX_FAILURE);
		if (change_state(this, FORK))
			return ((void)pthread_mutex_unlock(&this->shared->forks_lock \
			[this->next_fork]), STARVED);
		if (pthread_mutex_lock(&this->shared->forks_lock[this->number]))
		{
			pthread_mutex_unlock(&this->shared->forks_lock[this->next_fork]);
			return (MUTEX_FAILURE);
		}
		if (change_state(this, FORK))
			return ((void)pthread_mutex_unlock(&this->shared->forks_lock \
			[this->next_fork]), (void)pthread_mutex_unlock \
			(&this->shared->forks_lock[this->number]), STARVED);
		return (0);
	}
	else
		return (take_forks_mutex_odd(this));
}

int	get_forks(t_philo *this)
{
	if (take_forks_mutex(this))
		return (MUTEX_FAILURE);
	if (get_time() > this->starvation_time)
		return (STARVED);
	return (0);
}
