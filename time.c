/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 19:26:47 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:43:34 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}

void	ft_usleep(long long duration_ms, t_sim *sim)
{
	long long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < duration_ms)
	{
		if (sim && is_simulation_ended(sim))
			return ;
		usleep(500);
	}
}

/*
** Fills ts with "now + us microseconds" as an absolute time, the format
** expected by pthread_cond_timedwait (same clock as gettimeofday).
*/
void	timespec_from_now(struct timespec *ts, long us)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	us += tv.tv_usec;
	ts->tv_sec = tv.tv_sec + us / 1000000;
	ts->tv_nsec = (us % 1000000) * 1000;
}
