/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_states_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 22:51:58 by aurban            #+#    #+#             */
/*   Updated: 2024/01/08 04:52:30 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	change_state(t_philo *this, t_philo_state state)
{
	suseconds_t			time_stamp;
	static const char	state_str[][20] = {"has taken a fork", "is eating", \
	"is sleeping", "is thinking", "died"};

	time_stamp = (get_time() - this->shared.sim_data.start_time) / 1000;
	sem_wait(this->print_lock);
	printf("%-12ld %-3d\t%s\n", time_stamp, \
		this->number + 1, state_str[state]);
	if (state == DIE)
	{
		free(this);
		exit(SUB_PROCESS_SUCCESS);
	}
	sem_post(this->print_lock);
}
