/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurban <aurban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 01:30:59 by aurban            #+#    #+#             */
/*   Updated: 2023/12/13 17:13:23 by aurban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	while (*nptr && *nptr >= 48 && *nptr <= 57)
	{
		result *= 10;
		result += sign * (*nptr++ - 48);
	}
	return (result);
}

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	max;

	max = -1;
	if (size != 0 && ((max) / size) < nmemb)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (ptr)
		memset(ptr, 0, nmemb * size);
	return (ptr);
}

suseconds_t	get_time(void)
{
	struct timeval	var;

	gettimeofday(&var, NULL);
	return (var.tv_usec + (var.tv_sec * 1000000));
}

int	ft_usleep(t_philo *this, suseconds_t t)
{
	suseconds_t		wait_target;
	suseconds_t		time_now;
	struct timeval	var;

	wait_target = get_time() + t;
	while (this != NULL)
	{
		gettimeofday(&var, NULL);
		time_now = var.tv_usec + (var.tv_sec * 1000000);
		if (time_now - this->last_meal > this->sim_data->time_to_die)
		{
			change_state(this, DIE);
			return (1);
		}
		else if (time_now >= wait_target)
			return (0);
	}
	while (1)
	{
		gettimeofday(&var, NULL);
		time_now = var.tv_usec + (var.tv_sec * 1000000);
		if (time_now >= wait_target)
			return (0);
	}
}
