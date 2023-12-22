/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/22 18:03:37 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	routine_loop(t_philo *this)
{
	int			status;

	while (this->meal_count < this->shared.sim_data.meal_max)
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
		think(this);
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

static void	synchronize_processes(t_philo *this)
{
	this->last_meal = get_time();
	if ((this->number + 1) % 2 == 0)
	{
		if (this->shared.sim_data.philo_count % 2 == 1 && \
			this->number + 1 == this->shared.sim_data.philo_count)
		{
			ft_usleep(this, this->shared.sim_data.time_to_eat + 1);
		}
		else
			ft_usleep(this, (this->shared.sim_data.time_to_eat / 2));
	}
	return ;
}

static void	exit_clean(t_philo *this, int stauts)
{
	free(this);
	exit(stauts);
}

void	philosopher_routine(void *this_)
{
	t_philo	*this;

	this = this_;
	wait_others();
	this->forks_count = sem_open(SEM_FORK, 0);
	if (this->forks_count == NULL)
		exit_clean(this, SUB_PROCESS_FAILURE);
	this->print_lock = sem_open(SEM_PRINT, 0);
	if (this->print_lock == NULL)
		exit_clean(this, SUB_PROCESS_SUCCESS);
	this->shared.sim_data.start_time = get_time();
	if (this->shared.sim_data.meal_max == 0)
		exit_clean(this, SUB_PROCESS_SUCCESS);
	if (this->shared.sim_data.philo_count == 1)
		do_one_philo(this);
	synchronize_processes(this);
	routine_loop(this);
	exit_clean(this, SUB_PROCESS_SUCCESS);
}
