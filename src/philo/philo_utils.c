/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:31:34 by aurban            #+#    #+#             */
/*   Updated: 2023/12/11 12:21:45 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	mutex_unlocker(t_philo *this)
{
	pthread_mutex_unlock(&this->forks[this->number]);
	pthread_mutex_unlock(&this->forks[(this->number + 1) % \
		this->sim_data->philo_count]);
}

int	did_i_starve(t_philo *this)
{
	if (get_time() - this->last_meal >= this->sim_data->time_to_die)
		return (1) ;
	return (0);
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
	error = pthread_mutex_lock(this->print_lock);
	if (!error)
	{
		if (this->death)
		{
			pthread_mutex_unlock(this->print_lock);			
			return (-2);
		}
		time_stamp = (get_time() - this->sim_data->start_time) / 1000;
		printf("%ld %d %s\n", time_stamp,\
			this->number, state_str[state]);
		if (state == DIE)
			*(this->death) = 1;
	}
	else
		write(2, MUTEX_LOCK_ERROR, MLE_LEN);
	pthread_mutex_unlock(this->print_lock);
	return (error);
}