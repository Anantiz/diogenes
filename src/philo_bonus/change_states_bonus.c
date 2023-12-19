/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_states_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:51:58 by aurban            #+#    #+#             */
/*   Updated: 2023/12/19 18:57:48 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	init_states_cuz_shitty_norm(char *state_str[])
{
	state_str[0] = "has taken a fork";
	state_str[1] = "is eating";
	state_str[2] = "is sleeping";
	state_str[3] = "is thinking";
	state_str[4] = "died";
}

int	change_state(t_philo *this, t_philo_state state)
{
	char		*state_str[5];
	suseconds_t	time_stamp;
	int			error;

	init_states_cuz_shitty_norm(state_str);
	time_stamp = (get_time() - this->shared->sim_data.start_time) / 1000;
	error = pthread_mutex_lock(this->shared->print_lock);
	if (!error)
	{
		if (this->shared->death)
		{
			pthread_mutex_unlock(this->shared->print_lock);
			return (DEATH_VAL);
		}
		printf("%ld %d %s\n", time_stamp, \
			this->number + 1, state_str[state]);
		if (state == DIE)
			this->shared->death = 1;
	}
	else
		write(2, MUTEX_LOCK_ERROR "05\n", MLE_LEN + 3);
	pthread_mutex_unlock(this->shared->print_lock);
	return (error);
}
