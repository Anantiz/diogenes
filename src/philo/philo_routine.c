/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/14 12:03:43 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Tries to lock both right and left fork for the calling thread */
static int	take_forks(t_philo *this)
{
	if (pthread_mutex_lock(&this->forks[this->number]))
	{
		write(2, MUTEX_LOCK_ERROR"02\n", MLE_LEN + 3);
		return (-1);
	}
	if (change_state(this, FORK))
		return (-1);
	if (pthread_mutex_lock(&this->forks[(this->number + 1) % \
		this->sim_data->philo_count]))
	{
		pthread_mutex_unlock(&this->forks[this->number]);
		write(2, MUTEX_LOCK_ERROR"01\n", MLE_LEN + 3);
		return (-1);
	}
	if (change_state(this, FORK))
		return (fork_unlocker(this) - 100);
	return (0);
}

/*
Check the state of the forks, and take both if they are avaliable


0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0

*/
static int	get_forks(t_philo *this)
{
	while (1)
	{
		if (did_i_starve(this))
		{	
			// printf("%d Get Forks\tI'm about to end this man whole career\n", this->number + 1);
			return (STARVED);
		}
		if (pthread_mutex_lock(this->forks_state_lock))
		{
			write(2, MUTEX_LOCK_ERROR"04\n", MLE_LEN + 3);
			return (-1);
		}
		if (this->forks_state[this->number] == 0 && this->forks_state \
			[(this->number + 1) % this->sim_data->philo_count] == 0)
		{
			this->forks_state[this->number] = 1;
			this->forks_state[(this->number + 1) % \
				this->sim_data->philo_count] = 1;
			pthread_mutex_unlock(this->forks_state_lock);
			return (take_forks(this));
		}
		pthread_mutex_unlock(this->forks_state_lock);
	}
	return (0);
}

/* Mutex LOCK[integer _semaphore]*/

static int	eat_then_sleep(t_philo *this)
{
	// if (did_i_starve(this))
	// {
	// 	fork_unlocker(this);
	// 	change_state(this, DIE);
	// 	return (1);
	// }
	if (change_state(this, EAT))
		return (1) ;
	this->last_meal = get_time();
	// pthread_mutex_lock(this->print_lock);
	// printf("Last meal: %ld\t", (this->last_meal - this->sim_data->start_time) / 1000);
	// pthread_mutex_unlock(this->print_lock);
	this->meal_count++;
	if (ft_usleep(this, this->sim_data->time_to_eat))		// Dinner Time !
		return (1);
	if (fork_unlocker(this))
		return (1);
	if (this->sim_data->time_to_sleep > 0)
	{
		if (change_state(this, SLEEP))
			return (1);
		if (ft_usleep(this, this->sim_data->time_to_sleep))	// Goes to sleep once done
			return (1);
	}
	return (0);
}

/*
Loops,
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

	while (this->meal_count < this->sim_data->meal_max)
	{
		status = get_forks(this);
		if (!status)					// Eats
		{
			if (eat_then_sleep(this))
				break ;
		}
		else if (status == STARVED)		// Dies if starving
		{
			change_state(this, DIE);
			break ;
		}
		else
			break ;
		think_time = ((this->sim_data->time_to_die - \
			this->sim_data->time_to_eat * 2) - \
			(this->sim_data->time_to_sleep));
		if (think_time > 0)
		{
			// pthread_mutex_lock(this->print_lock);
			// printf("Time to think %ld  % d Think\t\tI'm about to end this man whole career\n",think_time / 1000, this->number + 1);
			// pthread_mutex_unlock(this->print_lock);
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
	if (this->sim_data->meal_max == 0)
		return (free(this), NULL);
	if (this->sim_data->philo_count == 1)
		return (do_one_philo(this), free(this), NULL);
	while (*(this->wait))
		;
	if ((this->number + 1) % 2 == 0)
	{
		if (this->sim_data->philo_count % 2 == 1 && \
			this->number + 1 == this->sim_data->philo_count)
			ft_usleep(NULL, this->sim_data->time_to_eat);
		else
			ft_usleep(NULL, (this->sim_data->time_to_eat / 2));
	}	
	this->last_meal = get_time();
	routine_loop(this);
	return (free(this), NULL);
}

// 180 - 60 -60 = 60 re_think WHY THE FCUK U DO THIS TO UR MOM, UR BREAKING MY HEART