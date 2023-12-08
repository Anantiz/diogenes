/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_logs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:31:34 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 16:18:06 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
epoch	0001702047323000000 us
my_calc	0000001702047237850 ms
max_long	9223372036854775807
*/
int	change_state(t_philo *this, t_philo_state state)
{
	const char	*state_str[5];
	suseconds_t	time_stamp;
	int			error;

	state_str[0] = "has taken a fork";
	state_str[1] = "is eating";
	state_str[2] = "is sleeping";
	state_str[3] = "is thinking";
	state_str[4] = "died";
	error = pthread_mutex_lock(this->print_lock);
	if (!error)
	{
		time_stamp = get_time() / 1000;//(get_time() - this->sim_data->start_time) / 1000;
		printf("%ld %d %s\n", time_stamp,\
			this->number, state_str[state]);
	}
	else
		write(2, "Mutext Lock error, aborting\n", 29);
	pthread_mutex_unlock(this->print_lock);
	return (error);
}
