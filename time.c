/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 19:26:47 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/26 19:49:27 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}

void	ft_usleep(long long duration_ms)
{
	long long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < duration_ms)
		usleep(500);
}
