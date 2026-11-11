/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:02:38 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:42:33 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Called with print_lock held. After the end of the simulation, only the
** "burned out" message may still be printed.
*/
static int	can_print(t_sim *sim, int is_burnout)
{
	int	allowed;

	pthread_mutex_lock(&sim->sim_lock);
	allowed = (!sim->simulation_ended || is_burnout);
	pthread_mutex_unlock(&sim->sim_lock);
	return (allowed);
}

void	print_status(t_sim *sim, int id, const char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&sim->print_lock);
	if (can_print(sim, strcmp(status, "burned out") == 0))
	{
		timestamp = get_time_in_ms() - sim->start_time;
		printf("%lld %d %s\n", timestamp, id, status);
	}
	pthread_mutex_unlock(&sim->print_lock);
}

/*
** The two "has taken a dongle" lines and "is compiling" are printed under
** a single print_lock, so they are always consecutive in the output.
*/
void	print_compile(t_sim *sim, int id)
{
	long long	timestamp;

	pthread_mutex_lock(&sim->print_lock);
	if (can_print(sim, 0))
	{
		timestamp = get_time_in_ms() - sim->start_time;
		printf("%lld %d has taken a dongle\n", timestamp, id);
		printf("%lld %d has taken a dongle\n", timestamp, id);
		printf("%lld %d is compiling\n", timestamp, id);
	}
	pthread_mutex_unlock(&sim->print_lock);
}
