/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:31:34 by aurban            #+#    #+#             */
/*   Updated: 2023/12/14 12:00:46 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	fork_unlocker(t_philo *this)
{
	pthread_mutex_unlock(&this->forks[this->number]);
	pthread_mutex_unlock(&this->forks[(this->number + 1) % \
		this->sim_data->philo_count]);
	if (!pthread_mutex_lock(this->forks_state_lock))
	{
		this->forks_state[this->number] = 0;
		this->forks_state[(this->number + 1) % \
			this->sim_data->philo_count] = 0;
		pthread_mutex_unlock(this->forks_state_lock);
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
	if (get_time() - this->last_meal > this->sim_data->time_to_die)
		return (1);
	return (0);
}

// Handles the special case where there is only one philosopher
void	do_one_philo(t_philo *this)
{
	
	if (change_state(this, FORK))
		return ;
	usleep(this->sim_data->time_to_die);
	change_state(this, DIE);
	return ;
}

static void	init_states_cuz_shitty_norm(char *state_str[])
{
	state_str[0] = "has taken a fork";
	state_str[1] = "is eating";
	state_str[2] = "is sleeping";
	state_str[3] = "is thinking";
	state_str[4] = "died";
}

/*
epoch	0001702047323000000 us
my_calc	0000001702047237850 ms
max_long	9223372036854775807
*/
int	change_state(t_philo *this, t_philo_state state)
{
	char		*state_str[5];
	suseconds_t	time_stamp;
	int			error;

	init_states_cuz_shitty_norm(state_str);
	time_stamp = (get_time() - this->sim_data->start_time) / 1000;
	error = pthread_mutex_lock(this->print_lock);
	if (!error)
	{
		if (*(this->death))
		{
			pthread_mutex_unlock(this->print_lock);
			return (DEATH_VAL);
		}
		printf("%ld %d %s\n", time_stamp, \
			this->number + 1, state_str[state]);
		if (state == DIE)
			*(this->death) = 1;
	}
	else
		write(2, MUTEX_LOCK_ERROR "05\n", MLE_LEN + 3);
	pthread_mutex_unlock(this->print_lock);
	return (error);
}
