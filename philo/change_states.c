/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_states.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:51:58 by aurban            #+#    #+#             */
/*   Updated: 2024/01/05 15:00:22 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	change_state(t_philo *this, t_philo_state state)
{
	int					error;
	suseconds_t			time_stamp;
	static const char	state_str[][20] = {"has taken a fork", "is eating", \
		"is sleeping", "is thinking", "died"};

	time_stamp = (get_time() - this->shared->sim_data.start_time) / 1000;
	error = pthread_mutex_lock(this->shared->print_lock);
	if (!error)
	{
		if (this->shared->death)
		{
			pthread_mutex_unlock(this->shared->print_lock);
			return (DEATH_VAL);
		}
		printf("%-12ld %-3d\t%s\n", time_stamp, \
			this->number + 1, state_str[state]);
		if (state == DIE)
			this->shared->death = 1;
	}
	else
		write(2, MUTEX_LOCK_ERROR "05\n", MLE_LEN + 3);
	pthread_mutex_unlock(this->shared->print_lock);
	return (error);
}
