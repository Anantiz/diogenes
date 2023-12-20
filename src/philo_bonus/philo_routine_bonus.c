/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/20 21:37:11 by aurban           ###   ########.fr       */
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
		change_state(this, THINK);
		if (ft_usleep(this, think_time) == DEATH_VAL)
			return (1);
	}
	if (did_i_starve(this))
		change_state(this, DIE);
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
			if (did_i_starve(this))
				change_state(this, DIE);
			if (eat_then_sleep(this) == DEATH_VAL)
				break ;
		}
		else if (status == DEATH_VAL)
			change_state(this, DIE);
		else
			break ;
		if (think(this))
			break ;
	}
	return ;
}

static void	wait_others(void)
{
	sem_t	*my_sem_wait;

	my_sem_wait = SEM_FAILED;
	while (my_sem_wait == SEM_FAILED)
	{
		my_sem_wait = sem_open(SEM_WAIT, 0);
	}
	sem_wait(my_sem_wait);
	sem_close(my_sem_wait);
}

static int	synchronize_table(t_philo *this)
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

	this = this_;
	wait_others();
	this->shared->sim_data.start_time = get_time();
	if (this->shared->sim_data.meal_max == 0)
		return (free(this), 0);
	if (this->shared->sim_data.philo_count == 1)
		do_one_philo(this);
	this->forks_count = sem_open(SEM_FORK, 0);
	if (this->forks_count == 0)
		return (free(this), -1);
	if (synchronize_table(this))
		return (free(this), -SUB_PROCESS_SUCCES);
	routine_loop(this);
	free(this);
	exit(SUB_PROCESS_SUCCES);
}
