/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_logs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:31:34 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 12:59:12 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	change_state(t_philo *this, t_philo_state state)
{
	const char	*state_str[5];
	int			error;

	state_str[0] = "has taken a fork";
	state_str[1] = "is eating";
	state_str[2] = "is sleeping";
	state_str[3] = "is thinking";
	state_str[4] = "died";
	error = pthread_mutex_lock(this->print_lock);
	if (!error)
		printf("%ld : %d %s\n", get_time(), this->number, state_str[state]);
	else
		write(2, "Mutext Lock error, aborting\n", 29);
	pthread_mutex_unlock(this->print_lock);
	return (error);
}
