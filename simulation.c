/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 01:10:33 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:43:22 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*lonely_coder(t_coder *coder);
static void	compile(t_coder *coder);
static void	debug_and_refactor(t_coder *coder);

int	is_simulation_ended(t_sim *sim)
{
	int	ended;

	pthread_mutex_lock(&sim->sim_lock);
	ended = sim->simulation_ended;
	pthread_mutex_unlock(&sim->sim_lock);
	return (ended);
}

/*
** Even coders start half a compile later, so odd coders join the dongle
** queues first: neighbours alternate instead of all queuing at once.
*/
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->sim->number_of_coders == 1)
		return (lonely_coder(coder));
	if (coder->coder_id % 2 == 0)
		ft_usleep(coder->sim->time_to_compile / 2, coder->sim);
	while (!is_simulation_ended(coder->sim))
	{
		if (!take_both_dongles(coder))
			break ;
		compile(coder);
		free_both_dongles(coder);
		debug_and_refactor(coder);
	}
	return (NULL);
}

/*
** With a single coder there is only one dongle: it can be taken but
** never paired, so the coder waits until the monitor detects burnout.
*/
static void	*lonely_coder(t_coder *coder)
{
	print_status(coder->sim, coder->coder_id, "has taken a dongle");
	while (!is_simulation_ended(coder->sim))
		usleep(500);
	return (NULL);
}

static void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_time = get_time_in_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->coder_lock);
	print_compile(coder->sim, coder->coder_id);
	ft_usleep(coder->sim->time_to_compile, coder->sim);
}

static void	debug_and_refactor(t_coder *coder)
{
	print_status(coder->sim, coder->coder_id, "is debugging");
	ft_usleep(coder->sim->time_to_debug, coder->sim);
	print_status(coder->sim, coder->coder_id, "is refactoring");
	ft_usleep(coder->sim->time_to_refactor, coder->sim);
}
