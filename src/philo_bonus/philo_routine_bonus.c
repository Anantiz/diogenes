/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/20 18:13:51 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static int	think(t_philo *this)
{
	long	think_time;

		think_time = this->shared->sim_data.time_to_eat - \
			this->shared->sim_data.time_to_sleep - 5;
	if (this->shared->sim_data.philo_count % 2)
		think_time += this->shared->sim_data.time_to_eat;
	if (think_time > 0)
	{
		if (change_state(this, THINK) == DEATH_VAL)
			return (1);
		if (ft_usleep(this, think_time) == DEATH_VAL)
			return (1);
	}
	return (0);
}

static void	routine_loop(t_philo *this)
{
	int			status;

	while (this->meal_count < this->shared->sim_data.meal_max)
	{
		status = get_forks(this);
		if (!status)
		{
			if (eat_then_sleep(this) == DEATH_VAL)
				break ;
		}
		else if (status == DEATH_VAL)
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

// static void	wait_family(t_philo *this)
// {
// 	int		wait;

// 	wait = 1;
// 	while (wait)
// 	{
// 		pthread_mutex_lock(this->shared->forks_lock);
// 		if (this->shared->wait == 0)
// 		{
// 			pthread_mutex_unlock(this->shared->forks_lock);
// 			return ;
// 		}
// 		pthread_mutex_unlock(this->shared->forks_lock);
// 	}
// }

static int	synchronize_family(t_philo *this)
{
	this->last_meal = get_time();
	if ((this->number + 1) % 2 == 0)
	{
		if (this->shared->sim_data.philo_count % 2 == 1 && \
			this->number + 1 == this->shared->sim_data.philo_count)
		{
			if (ft_usleep(this, this->shared->sim_data.time_to_eat + 1) \
				== DEATH_VAL)
				return (free(this), 1);
		}
		else
		{
			if (ft_usleep(this, (this->shared->sim_data.time_to_eat / 2)) \
				== DEATH_VAL)
				return (free(this), 1);
		}
	}
	return (0);
}

int	philosopher_routine(void *this_)
{
	t_philo	*this;

	this->shared->sim_data.start_time = get_time();
	this = this_;
	// wait_family(this);
	this->forks_count = sem_open(SEM_FORK, O_)
	if (this->shared->sim_data.meal_max == 0)
		return (free(this), 0);
	if (this->shared->sim_data.philo_count == 1)
		return (do_one_philo(this), free(this), 0);
	if (synchronize_family(this))
		return (1);
	routine_loop(this);
	return (free(this), 0);
}
