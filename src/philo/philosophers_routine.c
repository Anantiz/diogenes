/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:44:50 by aurban            #+#    #+#             */
/*   Updated: 2023/12/08 06:18:23 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_routine(void *this_)
{
	t_philo	*this;

	this = this_;
	if (pthread_mutex_lock(this->print_lock))
		printf("%d :I am %ld\n", this->number, this->thread_id);
	return (free(this), NULL);
}
