/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/13 13:08:41 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Tries to lock both right and left fork for the calling thread */
static int	take_forks(t_philo *this)
{
	int	error;
	
	error = pthread_mutex_lock(&this->forks[this->number]);
	if (error)
	{
		write(2, MUTEX_LOCK_ERROR"02\n", MLE_LEN + 3);
		return (-1);
	}
	if (change_state(this, FORK))
		return (-1);
	error = pthread_mutex_lock(&this->forks[(this->number + 1) \
		% this->sim_data->philo_count]);
	if (error)
	{
		pthread_mutex_unlock(&this->forks[this->number]);
		write(2, MUTEX_LOCK_ERROR"01\n", MLE_LEN + 3);
		return (-1);
	}
	if (change_state(this, FORK))
		return (fork_unlocker(this) - 100);
	return (0);
}

//Check if forks are available
static int	get_forks(t_philo *this)
{
	int	error;

	while (1)
	{
		error = pthread_mutex_lock(this->forks_state_lock);
		if (error)
		{
			write(2, MUTEX_LOCK_ERROR"04\n", MLE_LEN + 3);
			return (-1);
		}
		if (this->forks_state[this->number] == 0 && this->forks_state\
			[(this->number + 1) % this->sim_data->philo_count] == 0)
		{
			this->forks_state[this->number] = 1;
			this->forks_state[(this->number + 1) % \
				this->sim_data->philo_count] = 1;
			pthread_mutex_unlock(this->forks_state_lock);
			return (take_forks(this));
		}
		pthread_mutex_unlock(this->forks_state_lock);
		if (did_i_starve(this))
			return (STARVED);
	}
	return (0);
}

static int	eat_then_sleep(t_philo *this)
{
	if (did_i_starve(this))
	{
		fork_unlocker(this);
		change_state(this, DIE);
		return (1) ;
	}
	if (change_state(this, EAT))
		return (1) ;
	ft_usleep(this->sim_data->time_to_eat);		// Dinner Time !
	this->last_meal = get_time();
	this->meal_count++;
	if (fork_unlocker(this))
		return (1) ;
	if (change_state(this, SLEEP))
		return (1) ;
	ft_usleep(this->sim_data->time_to_sleep);	// Goes to sleep once done
	return (0);
}

static void	routine_loop(t_philo *this)
{
	int		status;

	while (this->meal_count < this->sim_data->meal_max)
	{
		status = get_forks(this);
		if (!status)					// Eats
		{
			if (eat_then_sleep(this))
				break ;

		}
		else if (status == STARVED)	// Dies if starving
		{
			change_state(this, DIE);
			break ;
		}
		else
			break ;
		if (change_state(this, THINK))
			break ;
	}
	return ;
}

void	*philosopher_routine(void *this_)
{
	t_philo	*this;

	this = this_;
	if (this->sim_data->philo_count == 1)
		return (do_one_philo(this), free(this), NULL);
	while (*(this->wait))
		;
	if (change_state(this, THINK))
		return (free(this), NULL) ;
	if ((this->number + 1) % 2 == 0)
		ft_usleep((this->sim_data->time_to_eat / 2) + 1);
	this->last_meal = get_time();
	routine_loop(this);
	return (free(this), NULL);
}

/*
check_order ->	|N 1 N 3 2 4|
				|  5	 6  |
state ->		|0 1 0 0 1 0|
How do I stop this ?

---------------------------
0 2 has taken a fork
0 2 has taken a fork
0 5 has taken a fork
0 5 has taken a fork
0 5 is eating
0 2 is eating
7 5 is sleeping
7 2 is sleeping
7 4 has taken a fork
7 4 has taken a fork
7 6 has taken a fork
7 6 has taken a fork
7 6 is eating
7 4 is eating
14 5 is thinking
14 2 is thinking
14 2 has taken a fork
14 2 has taken a fork
14 2 is eating
14 6 is sleeping
14 5 has taken a fork
14 5 has taken a fork
14 5 is eating
14 4 is sleeping
15 3 died
*/


/*
check_order ->	|1 N 3 2 4 N|

state ->		|1 0 1 1 1 0|

2 4 has taken a fork
2 4 has taken a fork
2 4 is eating
2 1 has taken a fork
2 1 has taken a fork
2 1 is eating
9 4 is sleeping
9 1 is sleeping
10 3 has taken a fork
10 3 has taken a fork
10 3 is eating
10 5 has taken a fork
10 5 has taken a fork
10 5 is eating
16 4 is thinking
16 1 is thinking
17 1 has taken a fork
17 1 has taken a fork
17 1 is eating
17 3 is sleeping
17 4 has taken a fork
17 4 has taken a fork
17 4 is eating
17 5 is sleeping
17 2 died
*/