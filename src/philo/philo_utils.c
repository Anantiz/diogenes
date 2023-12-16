/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:49:42 by aurban            #+#    #+#             */
/*   Updated: 2023/12/16 17:55:21 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	fork_unlocker(t_philo *this)
{
	pthread_mutex_unlock(&this->shared->forks[this->number]);
	pthread_mutex_unlock(&this->shared->forks[(this->number + 1) % \
		this->shared->sim_data.philo_count]);
	if (!pthread_mutex_lock(this->shared->forks_state_lock))
	{
		this->shared->forks_state[this->number] = 0;
		this->shared->forks_state[(this->number + 1) % \
			this->shared->sim_data.philo_count] = 0;
		pthread_mutex_unlock(this->shared->forks_state_lock);
		return (0);
	}
	else
	{
		write(2, MUTEX_LOCK_ERROR"08\n", MLE_LEN + 3);
		return (-1);
	}
}

int	did_i_starve(t_philo *this)
{
	if (get_time() - this->last_meal > this->shared->sim_data.time_to_die)
		return (1);
	return (0);
}

// Handles the special case where there is only one philosopher
void	do_one_philo(t_philo *this)
{
	
	if (change_state(this, FORK))
		return ;
	usleep(this->shared->sim_data.time_to_die);
	change_state(this, DIE);
	return ;
}

static suseconds_t	get_sleep_time(suseconds_t a, suseconds_t b)
{
	if (a < b)
	{
		a -= 2;
		if (a < 0)
			return (0);
		return (a);
	}
	b -= 2;
	if (b < 0)
		return (0);
	return (b);
}

int	ft_usleep(t_philo *this, suseconds_t t)
{
	suseconds_t		wait_target;
	suseconds_t		time_now;
	struct timeval	var;

	wait_target = get_time() + t;
	while (this == NULL)
	{
		gettimeofday(&var, NULL);
		time_now = var.tv_usec + (var.tv_sec * 1000000);
		if (time_now >= wait_target)
			return (0);
	}
	usleep((int)get_sleep_time(t, this->shared->sim_data.time_to_die));
	while (1)
	{
		gettimeofday(&var, NULL);
		time_now = var.tv_usec + (var.tv_sec * 1000000);
		if (time_now - this->last_meal > this->shared->sim_data.time_to_die)
		{
			change_state(this, DIE);
			return (1);
		}
		else if (time_now >= wait_target)
			return (0);
	}
}
